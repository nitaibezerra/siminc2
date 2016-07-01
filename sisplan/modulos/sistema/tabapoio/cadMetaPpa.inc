<?php
$mppid = $_REQUEST['mppid'];

$metaPpa = new MetaPpa($mppid);
extract($metaPpa->getDados());

if ($_POST['act'] == 'salvar'){

	$dado = array('mppcod'			=> $_REQUEST['mppcod'],
                  'mppnome'			=> $_REQUEST['mppnome'],
                  'prsano'			=> $_SESSION['exercicio'],
                  'mppdescricao'	=> $_REQUEST['mppdescricao']);

    if( !$mppid ){
    	$dado['mppstatus'] 		 = 'A';
    }

    $metaPpa->popularDadosObjeto($dado);
    $metaPpa->salvar();
    $metaPpa->commit();

    die("<script>
            alert('Operação realizada com sucesso!');
            location.href = 'sisplan.php?modulo=sistema/tabapoio/metaPpa&acao=A';
        </script>");
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba($abacod_tela,$url,$parametros);
$titulo = (empty($mppid) ? 'Cadastro de Meta PPA' : 'Alteração de Meta PPA');
monta_titulo( $titulo, obrigatorio() . 'Indica Campo Obrigatório.' );
?>

<form method="POST" name="formulario" action="">
<input type="hidden" name="mppid" value="<?php echo $mppid ?>"/>
<input type="hidden" name="evento" value=""/>
<input type=hidden name="act" value="0"/>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
        <td class="SubTituloDireita" width="30%"> Código:</td>
        <td>
        <?
            echo campo_texto('mppcod','S','','',20,2,'','');
        ?>
        </td>
    </tr>
	<tr>
        <td class="SubTituloDireita" width="30%"> Nome:</td>
        <td>
			<?= campo_textarea('mppnome', 'S', 'S', '', 69, 5, 400 ); ?>
        </td>
    </tr>
    <tr>
        <td align='right' class="SubTituloDireita"> Descrição:</td>
        <td>
              <?= campo_textarea('mppdescricao', 'S', 'S', '', 69, 5, 500 ); ?>
        </td>
    </tr>
    <tr style="background-color: #cccccc">
    <td align='right' style="vertical-align:top; width:30%"> </td>
    <td>
        <input type="button" name="salvar" value="Salvar" onclick="validar_cadastro();"/>
        <input type="button" name="Voltar" value="Voltar" onclick="javascript:location.href='sisplan.php?modulo=sistema/tabapoio/metaPpa&acao=A'"/>
    </td>
    </tr>
</table>
</form>


<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script language="javascript" type="text/javascript" >

function validar_cadastro(cod){
    if (!validaBranco(document.formulario.mppcod, 'Código')) return;
    if (!validaBranco(document.formulario.mppnome, 'Nome')) return;
    if (!validaBranco(document.formulario.mppdescricao, 'Descrição')) return;

    document.formulario.act.value = 'salvar';
    document.formulario.submit();
}

</script>

