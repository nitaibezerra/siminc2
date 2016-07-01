<?php
// monta cabeçalho

include APPRAIZ . 'includes/cabecalho.inc';

include APPRAIZ."sca/classes/Visitante.class.inc";
$oVisitante = new Visitante();

print '<br/>';

// tipo de consulta visitante
$tipoConsulta = 'V';
monta_titulo( 'Consulta de Visitantes', '&nbsp;' );

?>


<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language='javascript' type='text/javascript' src='./js/sca.js'></script>
<script language="javascript" type="text/javascript" src="./js/ajax.js"></script>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script language="javascript" type="text/javascript">

    //função que dispara a pesquisa no banco
    function pesquisar(acao){
        var url = 'sca.php?modulo=principal/visitante/atualizarVisitante&acao=A&pesquisar='+acao;
        var formulario=document.getElementById("formulario");
        formulario.action = url;
        formulario.submit();
    }

    function exibirMensagem(msg){
        alert(msg);
    }

    //função que abre em popup a tela para edição do registro de visitante
    function editarVisitante(vttid){

        if(vttid != ''){
            AbrirPopUp('?modulo=principal/visitante/popup/cadastrarVisitante&acao=A&vttid='+vttid+'&alteracao=tipo', 'cadastrarVisitante', 'scrollbars=yes, width=1000, height=380');
        }
        else{
            alert('Visitante não selecionado');
        }
    }

</script>

<form name="formulario" id="formulario" method="post">
    <table class="tabela" align="center" cellpadding="2" cellspacing="1" bgcolor="#f5f5f5" width="100%">
        <tr>
            <td class="SubTituloDireita">Número do Documento:</td>
            <td><?=campo_texto('vttdoc','','','',30,20,'','','left','',0,'id="vttdoc"','this.value=formatarNumeroDocumento(this.value,null);',$_POST['vttdoc'],'this.value=formatarNumeroDocumento(this.value,null);');?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Nome:</td>
            <td><?=campo_texto('vttnome','','','',60,100,'','','left','',0,'id="vttnome"','',$_POST['vttnome']);?></td>
        </tr>
        <tr>
            <td bgcolor="#CCCCCC" colspan="2" align="center">
                <input type="button" name="visualizar" value="Visualizar" style="cursor:pointer;" onclick="pesquisar('filtrar');">
                <input type="button" name="vertodos" value="Ver Todos" style="cursor:pointer;" onclick="pesquisar('todos');">
            </td>
        </tr>
    </table>
    <input type='hidden' id='tipoConsulta' name='tipoConsulta' value='V'>
</form>

            
<?php
if($_POST['pesquisar'] == 'filtrar'){
    $oVisitante->listarVisitantes($_POST['vttdoc'], $_POST['vttnome'], $_POST['no_unidade_org'],$tipoConsulta);
}else{
    $oVisitante->listarVisitantes('', '', '',$tipoConsulta);
}
?>
