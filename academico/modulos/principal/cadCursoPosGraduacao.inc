<?php

require_once APPRAIZ . "academico/classes/CursoPosGraduacao.class.inc";
require_once APPRAIZ . "academico/classes/NivelCurso.class.inc";

monta_titulo("Cadastro de Programa", "");

$oCursoPosGraduacao = new CursoPosGraduacao();
$oNivelCurso = new NivelCurso();

if(isset($_POST['btnSalvar'])){

    $cpgidRetorno = $oCursoPosGraduacao->salvar($_POST);

    if($cpgidRetorno){

        $_POST['cpgid'] = $cpgidRetorno;
        alert('Salvo com Sucesso.');
        echo "<script type='text/javascript'>
                    window.close();
                    window.opener.location = '?modulo=principal/pesquisaCursoPosGraduacao&acao=A';
              </script>";

    }else{

        alert('O Código ou Nome do Programa já existe no Banco.');

    }
    
    extract($_POST);

}else if(!empty($_GET['cpgid'])){
    
    $oCursoPosGraduacao->carregarPorId($_GET['cpgid']);
    $dados = $oCursoPosGraduacao->getDados();

    extract($dados);

}

if($_GET['entid'])
    $entid = $_GET['entid'];

?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language='javascript' type='text/javascript' src='../includes/funcoes.js'></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<script type="text/javascript">

    /**
     * Valida a integridade dos dados
     * @name validaFormulario
     * @author Silas Matheus
     * @access public
     * @return bool
     */
    function validaFormulario(){

        var resultado = false;

        if($('#cpgcodigo').val() == ''){

            alert('Preencha o Código do Programa');
            $('#cpgcodigo').focus();

        }else if($('#cpgdsc').val() == ''){

            alert('Preencha o Nome do Programa');
            $('#cpgdsc').focus();

        }else if( $('#nvcid').val() == ''){

            alert('Selecione o Nível do Programa');
            $('#nvcid').focus();

        }else{

            resultado = true;

        }

        return resultado;

    }

</script>

<form id="formulario" name="formulario" action="" method="post" onSubmit="return validaFormulario();" enctype="multipart/form-data" >

    <table id="tblCurso" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="SubTituloDireita" style="width:100px;"> C&oacute;digo: </td>
            <td> <?php echo campo_texto('cpgcodigo', 'S', 'S', '', 15, 15, '', '', '', '', '', 'id="cpgcodigo"'); ?> </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"> Nome: </td>
            <td> <?php echo campo_texto('cpgdsc', 'S', 'S', '', 80, 100, '', '', '', '', '', 'id="cpgdsc"'); ?> </td>
        </tr>
        <tr>
            <td class="SubTituloDireita"> N&iacute;vel: </td>
            <td> <?php $oNivelCurso->montaCombo($nvcid, $_GET['cpgid'] ? 'N' : 'S'); ?> </td>
        </tr>
        <tr>
            <th colspan="2" style="text-align: center;">
                <input type="hidden" value="<?php echo $entid; ?>" name="entid" id="entid" />
                <input type="hidden" value="<?php echo $cpgid; ?>" name="cpgid" id="cpgid" />
                <?php if( $db->testa_superuser() ): ?>
                <input type="submit" value="Salvar" name="btnSalvar" id="btnSalvar" />
                <?php endif; ?>
                <input type="button" value="Voltar" name="btnVoltar" id="btnVoltar" onclick="window.close();" />
            </th>
        </tr>
    </table>

</form>