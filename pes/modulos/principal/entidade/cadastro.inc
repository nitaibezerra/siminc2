<?php

//echo "<pre>";
//var_dump(file_exists(''));
//exit;

$controllerAcao = new Controller_Entidade();

$action = (isset($_POST['action'])) ? $_POST['action'] : null;
switch ($action) :
    case 'salvar':
        echo $controllerAcao->salvarAction();
        break;
    case 'formulario':
        echo $controllerAcao->formularioAction();
        break;
    case 'listarUO':
        echo $controllerAcao->listarUOAction();
        break;
    case 'listarEntidade':
        echo $controllerAcao->listarEntidadeAction();
        break;
    default:
        //Chamada de programa
        include APPRAIZ . "includes/cabecalho.inc";
        ?> 
        <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
        <br />
        <table align="center" width="95%" border="0" cellpadding="0" cellspacing="0" class="listagem2" style="text-align: center;">
            <tr bgcolor="#e7e7e7">
                <td><h1>Cadastro de Entidades</h1></td>
            </tr>
            <tr class="container_formulario" bgcolor="#e7e7e7" >
            </tr>
            <tr bgcolor="#e7e7e7">
                <td>
                    <!--<hr size="1" noshade="noshade" color="#dddddd" style="margin:15px 0 15px 0;" />--> 
                    <div class="ativo" id="arvore" style="opacity: 1;">   
                        <?php $controllerAcao->listarOrgaoAction(); ?>
                    </div>
                </td>
            </tr>
        </table>
        <?php
        break;
endswitch;
?>
