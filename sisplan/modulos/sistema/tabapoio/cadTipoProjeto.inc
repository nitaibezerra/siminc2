<?php
$tppid = $_REQUEST['tppid'];

$tipoProjeto = new TipoProjetoPlanejamento($tppid);
extract($tipoProjeto->getDados());

if ($_POST['act'] == 'salvar'){

	$dado = array('tppnome'			=> $_REQUEST['tppnome'],
                  'tppdescricao'	=> $_REQUEST['tppdescricao'],
                  'tppsigla'		=> $_REQUEST['tppsigla'],
                  'dtcriacao'		=> 'now()');

    if( !$tppid ){
    	$dado['tppstatus'] 		 = 'A';
    }

    $tipoProjeto->popularDadosObjeto($dado);
    $tipoProjeto->salvar();
    $tipoProjeto->commit();

    die("<script>
            alert('Operação realizada com sucesso!');
            location.href = '?modulo=sistema/tabapoio/tipoProjeto&acao=A';
        </script>");
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba($abacod_tela,$url,$parametros);
$titulo = (empty($tppid) ? 'Cadastro de Tipo de Projeto' : 'Alteração de Tipo de Projeto');
monta_titulo( $titulo, obrigatorio() . 'Indica Campo Obrigatório.' );
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<form method="POST" name="formulario" action="">
<input type="hidden" name="tppid" value="<?php echo $tppid ?>"/>
<input type="hidden" name="evento" value=""/>
<input type=hidden name="act" value="0"/>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
        <td class="SubTituloDireita" width="30%"> Nome:</td>
        <td>
        <?
            echo campo_texto('tppnome','S','S','',51,200,'','');
        ?>
        </td>
    </tr>
	<tr>
        <td class="SubTituloDireita" width="30%"> Descrição:</td>
        <td>
        <?
           echo campo_textarea('tppdescricao', 'S', 'S', '', 69, 5, 500 );
        ?>
        </td>
    </tr>
    <tr>
        <td align='right' class="SubTituloDireita"> Sigla:</td>
        <td>
            <?= campo_texto('tppsigla','S','','',10,1,'',''); ?>
        </td>
    </tr>
    <tr style="background-color: #cccccc">
    <td align='right' style="vertical-align:top; width:30%"> </td>
    <td>
        <input type="button" name="salvar" value="Salvar" onclick="validar_cadastro();"/>
        <input type="button" name="Voltar" value="Voltar" onclick="javascript:location.href='?modulo=sistema/tabapoio/tipoProjeto&acao=A'"/>
    </td>
    </tr>
</table>
</form>


<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script language="javascript" type="text/javascript" >

function validar_cadastro(cod){
    if (!validaBranco(document.formulario.tppnome, 'Nome')) return;
    if (!validaBranco(document.formulario.tppdescricao, 'Descrição')) return;
    if (!validaBranco(document.formulario.tppsigla, 'Sigla')) return;

    document.formulario.act.value = 'salvar';
    document.formulario.submit();
}

</script>

