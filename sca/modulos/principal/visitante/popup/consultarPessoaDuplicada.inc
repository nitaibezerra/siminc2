<?php
include APPRAIZ."sca/classes/Visita.class.inc";

$oVisita	= new Visita();

monta_titulo( 'Consulta de Registros Duplicados', '&nbsp;' );
?>

<html>
<head></head>
<script language="javascript" type="text/javascript" src="./js/ajax.js"></script>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/prototype.js"></script>
<script language='javascript' type="text/javascript" src="./js/sca.js"></script>
<script language="javascript" type="text/javascript">

//carrega os dados da pessoa selecionada na tela chamadora
    function selecionarPessoa(vttid, nu_matricula_siape){
        var tipoConsulta = '<?=$tipoConsulta?>';
        executarScriptPai('carregarPessoa(' + vttid + ',' + nu_matricula_siape + ');');

        if(tipoConsulta != 'A'){
            self.close();
        }
    }
 
</script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<body>

<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
    <tr>
        <td colspan='2'>
            <div style='height:500px;overflow-y:scroll;'>
            <?php 
            	if($_REQUEST['saida']){
					$saida = true;
				} else {
					$saida = false;
				}
            	$oVisita->listarRegistroDuplicado($_REQUEST['vttdoc'], $_REQUEST['tipo'], $saida);
            ?>
            </div>
        </td>
    </tr>
</table>
</body>
</html>