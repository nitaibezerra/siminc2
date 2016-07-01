<?php

include APPRAIZ."sca/classes/Visitante.class.inc";
$oVisitante = new Visitante();

//pega o parâmetro que indidica se a consulta será apenas visitante, servidor ou todos
$tipoConsulta = $_REQUEST['tipoConsulta'];

switch ($tipoConsulta){
    case 'S': //S = apenas por servidor
        monta_titulo( 'Consulta de Servidores', '&nbsp;' );
        break;
    case 'V': //V = apenas por visitante
        monta_titulo( 'Consulta de Visitantes', '&nbsp;' );
        break;
    case 'T': //T = Todos
        monta_titulo( 'Consulta de Servidores e Visitantes', '&nbsp;' );
        break;
    case 'A': //A = Autorizados
        monta_titulo( 'Consulta de Autorizados', '&nbsp;' );
        break;
    default:
        $tipoConsulta = 'T';
        monta_titulo( 'Consulta de Servidores e Visitantes', '&nbsp;' );
        break;
}

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
        var url = 'sca.php?modulo=principal/visitante/popup/consultarPessoa&acao=A&pesquisar='+acao;
        var formulario=document.getElementById("formulario");
        formulario.action = url;
        formulario.submit();
    }

    //carrega os dados da pessoa selecionada na tela chamadora
    function selecionarPessoa(vttid, nu_matricula_siape, nome){
        var tipoConsulta = '<?=$tipoConsulta?>';
        executarScriptPai('carregarPessoa(' + vttid + ',' + nu_matricula_siape + ',"' + nome + '");');

        if(tipoConsulta != 'A'){
            self.close();
        }
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
            <td class="SubTituloDireita">Número do Documento:</td>
            <td><?=campo_texto('vttdoc','','','',30,20,'','','left','',0,'id="vttdoc"','this.value=formatarNumeroDocumento(this.value,null);',$_REQUEST['vttdoc'],'this.value=formatarNumeroDocumento(this.value,null);');?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Nome:</td>
            <td><?=campo_texto('vttnome','','','',60,100,'','','left','',0,'id="vttnome"','',$_REQUEST['vttnome']);?></td>
        </tr>
        <?php //Somente exibe filtro de unidade caso a pesquisa não seja para visitante.?>
		<?php if($tipoConsulta != 'V'):?>
        <tr>
            <td class="SubTituloDireita">Unidade:</td>
            <td><?=campo_texto('no_unidade_org','','','',60,100,'','','left','',0,'id="no_unidade_org"','',$_REQUEST['no_unidade_org']);?></td>
        </tr>
        <?php endif;?>
        <tr>
            <td bgcolor="#CCCCCC" colspan="2" align="center">
                <input type="button" name="visualizar" value="Visualizar" style="cursor:pointer;" onclick="pesquisar('filtrar');">
                <input type="button" name="vertodos" value="Ver Todos" style="cursor:pointer;" onclick="pesquisar('todos');">
            </td>
        </tr>
    </table>
    <input type='hidden' id='tipoConsulta' name='tipoConsulta' value='<?=$tipoConsulta?>'>
</form>
<script>
    Event.observe(window, 'load', function() {
        $("vttdoc").focus();
      });
</script>
<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
    <tr>
        <td colspan='2'>
            <div style='height:500px;overflow-y:scroll;'>
            <?php
            if($_REQUEST['pesquisar'] == 'filtrar'){
                $oVisitante->listarVisitantePorFiltro($_REQUEST['vttdoc'], $_REQUEST['vttnome'], $_REQUEST['no_unidade_org'],$tipoConsulta);
            }elseif ($_REQUEST['pesquisar'] == 'todos'){
                $oVisitante->listarVisitantePorFiltro('', '', '',$tipoConsulta);
            }
            ?>
            </div>
        </td>
    </tr>
</table>
</body>
</html>