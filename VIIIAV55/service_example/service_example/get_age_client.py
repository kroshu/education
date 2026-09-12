import rclpy
from interfaces.srv import GetAge
from rclpy.node import Node


class GetAgeClient(Node):

    def __init__(self):
        super().__init__("get_age_client")
        self.logger = self.get_logger()

        # TODO 1
        # Declare parameters for the birth date the client should send
        # ...

        # TODO 2
        # Create a client for the `get_age` service
        # ...

        # TODO 3
        # Wait for the service to become available
        # ...

    def send_request(self):
        # TODO 4
        # Send request to the server
        pass


def main():
    rclpy.init()

    # TODO 5
    # Create client, send request and wait for response
    # ...

    rclpy.shutdown()


if __name__ == "__main__":
    main()
