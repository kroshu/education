import sys

from interfaces.srv import GetAge
import rclpy
from rclpy.node import Node


class GetAgeClient(Node):

    def __init__(self):
        super().__init__('get_age_client')
        self.logger = self.get_logger()
        self.cli = self.create_client(GetAge, 'get_age')
        while not self.cli.wait_for_service(timeout_sec=1.0):
            self.logger.info('service not available, waiting again...')
        self.req = GetAge.Request()

        self.logger.info("GetAge client succesfully initialized")

    def send_request(self, birth_date):
        self.logger.info(f'Sending request to get_age service with birth date: {birth_date}')
        self.req.birth_date = birth_date
        return self.cli.call_async(self.req)


def main():
    rclpy.init()

    client = GetAgeClient()
    birth_date = "2003.03.02."
    future = client.send_request(birth_date)
    rclpy.spin_until_future_complete(client, future)
    response = future.result()
    client.logger.info(f'Result of get_age: {response.age}')

    client.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()