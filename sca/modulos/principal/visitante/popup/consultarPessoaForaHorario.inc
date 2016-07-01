<?php

include APPRAIZ."sca/classes/Visita.class.inc";
$oVisita = new Visita();

monta_titulo( 'Consulta de Servidores e Terceirizados', '&nbsp;' );

?>

<html>
<head></head>
<script language="javascript" type="text/javascript" src="./js/ajax.js"></script>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/prototype.js"></script>
<script language='javascript' type="text/javascript" src="./js/sca.js"></script>
<script language="javascript" type="text/javascript">

    //função que dispara a pesquisa no banco
    function pesquisar(acao){
        var url = 'sca.php?modulo=principal/visitante/popup/consultarPessoaForaHorario&acao=A&pesquisar='+acao;
        var formulario=document.getElementById("formulario");
        formulario.action = url;
        formulario.submit();
    }

    function fecharJanela(){
    	self.close();
    }

    //carrega os dados da pessoa selecionada na tela chamadora
    function selecionarPessoa(documento){
        executarScriptPai('carregarPessoaForaHorario("' + documento + '");');
    }

    function exibirMensagem(msg){
        alert(msg);
    }

</script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<body>
<form name="formulario" id="formulario" method="post">
    <table class="tabela" align="center" cellpadding="2" cellspacing="1" bgcolor="#f5f5f5" width="100%">
        <tr>
            <td class="SubTituloDireita">Documento:</td>
            <td><?=campo_texto('documento','','','',30,20,'','','left','',0,'id="vttdoc"','this.value=formatarNumeroDocumento(this.value,null);',$_REQUEST['documento'],'this.value=formatarNumeroDocumento(this.value,null);');?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Nome:</td>
            <td><?=campo_texto('nome','','','',60,100,'','','left','',0,'id="nome"','',$_REQUEST['nome']);?></td>
        </tr>
        <tr>
            <td bgcolor="#CCCCCC" colspan="2" align="center">
                <input type="button" name="visualizar" value="Visualizar" style="cursor:pointer;" onclick="pesquisar('filtrar');">
                <input type="button" name="vertodos" value="Ver Todos" style="cursor:pointer;" onclick="pesquisar('todos');">
            </td>
        </tr>
    </table>
</form>
<script>
    Event.observe(window, 'load', function() {
        $("documento").focus();
      });
</script>
<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
    <tr>
        <td colspan='2'>
            <div style='height:500px;overflow-y:scroll;'>
            <?php
            if($_REQUEST['pesquisar'] == 'filtrar'){
                $oVisita->consultarPessoaForaHorario($_REQUEST['documento'], $_REQUEST['nome'], true);
            }elseif ($_REQUEST['pesquisar'] == 'todos'){
                $oVisita->consultarPessoaForaHorario('', '', true);
            }
            ?>
            </div>
        </td>
    </tr>
    <tr>
    	<td bgcolor="#CCCCCC" colspan="2" align="center">
    		<input type="button" onclick="fecharJanela();" value="Voltar"/>
    	</td>
    </tr>
</table>
</body>
</html>