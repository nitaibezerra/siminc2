<?php
$to      = 'thiagotasca@gmail.com';
$subject = 'teste email demanda';
$message = 'hello';
$headers = 'From: thiagotasca@gmail.com' . "\r\n" .
    'Reply-To: webmaster@example.com' . "\r\n" .
    'X-Mailer: PHP/' . phpversion();

mail($to, $subject, $message, $headers);
?>