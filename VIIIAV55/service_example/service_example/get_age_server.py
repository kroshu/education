from datetime import date, datetime

import rclpy
from interfaces.srv import GetAge
from rclpy.node import Node


class GetAgeServer(Node):

    def __init__(self):
        super().__init__("get_age_server")
        self.logger = self.get_logger()

        # Create a server for the GetAge service
        self.srv = self.create_service(GetAge, "get_age", self.get_age_callback)

        self.logger.info("GetAge server succesfully initialized")

    def get_age_callback(self, request, response):
        self.logger.info("GetAge service request received")

        # Parse the birth date from the request and calculate the age
        birth_string = request.birth_date.lower().strip()
        birth = datetime.strptime(birth_string, "%Y.%m.%d.").date()

        today = date.today()
        response.age = (
            today.year
            - birth.year
            - ((today.month, today.day) < (birth.month, birth.day))
        )
        self.logger.info(
            f"For birth date {birth_string} sending back age: {response.age}"
        )

        return response


def main():
    rclpy.init()
    server = GetAgeServer()
    rclpy.spin(server)
    rclpy.shutdown()


if __name__ == "__main__":
    main()
