import rclpy
from interfaces.srv import GetAge
from rclpy.node import Node


class GetAgeClient(Node):

    def __init__(self):
        super().__init__("get_age_client")
        self.logger = self.get_logger()

        # Declare the birth date parameter
        self.declare_parameter("birth_date", "2003.03.02.")
        self.birth_date = (
            self.get_parameter("birth_date").get_parameter_value().string_value
        )

        # Create a client for the GetAge service
        self.cli = self.create_client(GetAge, "get_age")
        while not self.cli.wait_for_service(timeout_sec=1.0):
            self.logger.info("service not available, waiting again...")

        self.req = GetAge.Request()
        self.logger.info("GetAge client succesfully initialized")

    def send_request(self):
        self.logger.info(
            f"Sending request to get_age service with birth date: {self.birth_date}"
        )
        self.req.birth_date = self.birth_date
        return self.cli.call_async(self.req)


def main():
    rclpy.init()

    client = GetAgeClient()
    future = client.send_request()

    # Only spin until the future is complete
    rclpy.spin_until_future_complete(client, future)
    response = future.result()
    client.logger.info(f"Result of get_age: {response.age}")

    client.destroy_node()
    rclpy.shutdown()


if __name__ == "__main__":
    main()
