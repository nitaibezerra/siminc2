<?php
$formDocumento = new Ted_Form_FNDEDocumenta();

if (isset($_POST['logindoc']) && !$formDocumento->isValid($_POST)) {
    $formDocumento->reset();
}

echo $formDocumento->showForm();
?>