from datetime import date, datetime

import rclpy
from interfaces.srv import GetAge
from rclpy.node import Node


class GetAgeServer(Node):

    def __init__(self):
        super().__init__("get_age_server")
        self.logger = self.get_logger()

        # TODO 1
        # Create a server for the GetAge service
        # ...

    def get_age_callback(self, request, response):
        self.logger.info("GetAge service request received")

        # TODO 2
        # Parse the birth date from the request
        # ...

        # TODO 3
        # Calculate the user's age
        # ...

        return response


def main():
    rclpy.init()
    server = GetAgeServer()
    rclpy.spin(server)
    rclpy.shutdown()


if __name__ == "__main__":
    main()
