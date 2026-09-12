from launch_ros.actions import Node

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration


def generate_launch_description():
    declare_birth_date = DeclareLaunchArgument(
        "birth_date",
        default_value="2003.03.02.",
        description="Birth date to send to the get_age service",
    )

    server = Node(
        package="service_example",
        executable="server",
        name="server",
    )

    client = Node(
        package="service_example",
        executable="client",
        name="client",
        parameters=[
            {
                "birth_date": LaunchConfiguration("birth_date"),
            }
        ],
    )

    return LaunchDescription(
        [
            declare_birth_date,
            server,
            client,
        ]
    )
