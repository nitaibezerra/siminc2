<?php
$mpnid = $_REQUEST['mpnid'];

$metaPnc = new MetaPnc($mpnid);
extract($metaPnc->getDados());

if ($_POST['act'] == 'salvar'){

	$dado = array('mpncod'			=> $_REQUEST['mpncod'],
                  'mpnnome'			=> $_REQUEST['mpnnome'],
                  'prsano'			=> $_SESSION['exercicio'],
                  'mpndescricao'	=> $_REQUEST['mpndescricao']);

    if( !$mpnid ){
    	$dado['mpnstatus'] 		 = 'A';
    }

    $metaPnc->popularDadosObjeto($dado);
    $metaPnc->salvar();
    $metaPnc->commit();

    die("<script>
            alert('Operação realizada com sucesso!');
            location.href = 'sisplan.php?modulo=sistema/tabapoio/metaPnc&acao=A';
        </script>");
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba($abacod_tela,$url,$parametros);
$titulo = (empty($mpnid) ? 'Cadastro de Meta PNC' : 'Alteração de Meta PNC');
monta_titulo( $titulo, obrigatorio() . 'Indica Campo Obrigatório.' );
?>

<form method="POST" name="formulario" action="">
<input type="hidden" name="mpnid" value="<?php echo $mpnid ?>"/>
<input type="hidden" name="evento" value=""/>
<input type=hidden name="act" value="0"/>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
        <td class="SubTituloDireita" width="30%"> Código:</td>
        <td>
        <?
            echo campo_texto('mpncod','S','','',20,2,'','');
        ?>
        </td>
    </tr>
	<tr>
        <td class="SubTituloDireita" width="30%"> Nome:</td>
        <td>
			<?= campo_textarea('mpnnome', 'S', 'S', '', 69, 5, 400 ); ?>
        </td>
    </tr>
    <tr>
        <td align='right' class="SubTituloDireita"> Descrição:</td>
        <td>
              <?= campo_textarea('mpndescricao', 'S', 'S', '', 69, 5, 500 ); ?>
        </td>
    </tr>
    <tr style="background-color: #cccccc">
    <td align='right' style="vertical-align:top; width:30%"> </td>
    <td>
        <input type="button" name="salvar" value="Salvar" onclick="validar_cadastro();"/>
        <input type="button" name="Voltar" value="Voltar" onclick="javascript:location.href='sisplan.php?modulo=sistema/tabapoio/metaPnc&acao=A'"/>
    </td>
    </tr>
</table>
</form>


<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script language="javascript" type="text/javascript" >

function validar_cadastro(cod){
    if (!validaBranco(document.formulario.mpncod, 'Código')) return;
    if (!validaBranco(document.formulario.mpnnome, 'Nome')) return;
    if (!validaBranco(document.formulario.mpndescricao, 'Descrição')) return;

    document.formulario.act.value = 'salvar';
    document.formulario.submit();
}

</script>

