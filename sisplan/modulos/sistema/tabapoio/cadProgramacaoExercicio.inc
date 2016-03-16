<?php
$prsano = $_REQUEST['prsano_id'];

$programacaoExercicio = new ProgramacaoExercicio($prsano);
extract($programacaoExercicio->getDados());

if ($_POST['act'] == 'salvar'){

	if($_REQUEST['prsexerccorrente'] == 't'){
		$programacaoExercicio->inabilitaCorrente();
	}

    $dado = array('prsano'			   			=> $_REQUEST['prsano'],
                  'prsdata_inicial'	   			=> formata_data_sql($_REQUEST['prsdata_inicial']),
                  'prsdata_termino'	  			=> formata_data_sql($_REQUEST['prsdata_termino']),
                  'prsdtplanejamentoinicio'		=> formata_data_sql($_REQUEST['prsdtplanejamentoinicio']),
                  'prsdtplanejamentotermino'	=> formata_data_sql($_REQUEST['prsdtplanejamentotermino']),
    			  'prsexerccorrente'   			=> $_REQUEST['prsexerccorrente'],
                  'prsativo'           			=> ($_REQUEST['prsativo'] == 't' ? 1 : 0),
    			  'prsexercicioaberto' 			=> $_REQUEST['prsexercicioaberto']);

    $programacaoExercicio->gravarRegistro($dado);
    $programacaoExercicio->commit();

    die("<script>
            alert('Operação realizada com sucesso!');
            location.href = '?modulo=sistema/tabapoio/programacaoExercicio&acao=A';
        </script>");
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba($abacod_tela,$url,$parametros);
$titulo = (empty($prsano) ? 'Cadastro de Programação Exercício' : 'Alteração de  Programação Exercício');
monta_titulo( $titulo, obrigatorio() . 'Indica Campo Obrigatório.' );
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<form method="POST" name="formulario" action="">
<input type="hidden" name="prsano_id" value="<?php echo $prsano ?>"/>
<input type="hidden" name="evento" value=""/>
<input type=hidden name="act" value="0"/>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
     <tr>
         <td align='right' class="SubTituloDireita" style="width:25%;">Ano: </td>
         <td>
             <?php
                 echo campo_texto('prsano','S','','',11,4,'####','');
             ?>
         </td>
     </tr>
     <tr>
        <td align='right' class="SubTituloDireita">Data de Início:</td>
        <td>
        <?
            echo campo_data2('prsdata_inicial', 'S', 'S', '', 'S');
        ?>
        </td>
    </tr>
     <tr>
        <td align='right' class="SubTituloDireita">Data de Término:</td>
        <td>
        <?
            echo campo_data2('prsdata_termino', 'S', 'S', '', 'S');
        ?>
        </td>
    </tr>
         <tr>
        <td align='right' class="SubTituloDireita">Data de Início do Planejamento:</td>
        <td>
        <?
            echo campo_data2('prsdtplanejamentoinicio', 'S', 'S', '', 'S');
        ?>
        </td>
    </tr>
     <tr>
        <td align='right' class="SubTituloDireita">Data de Término do Planejamento:</td>
        <td>
        <?
            echo campo_data2('prsdtplanejamentotermino', 'S', 'S', '', 'S');
        ?>
        </td>
    </tr>
    <tr>
        <td align='right' class="SubTituloDireita">Exercício Corrente:</td>
        <td>
			<input type="radio" name="prsexerccorrente" id="prsexerccorrente-t" <?php echo $prsexerccorrente == 't' ? ' checked="checked" ' : "" ?> value="t" /> <label for="prsexerccorrente-t" style="cursor:pointer;"> Sim</label>
			<input type="radio" name="prsexerccorrente" id="prsexerccorrente-f" <?php echo ($prsexerccorrente == 'f' || empty($prsexerccorrente) ) ? ' checked="checked" ' : "" ?> value="f" /> <label for="prsexerccorrente-f" style="cursor:pointer;"> Não</label>
        	<?php echo obrigatorio(); ?>
        </td>
    </tr>
    <tr>
        <td align='right' class="SubTituloDireita">Exercício Ativo:</td>
        <td>
			<input type="radio" name="prsativo" id="prsativo-t" <?php echo $prsativo == '1' ? ' checked="checked" ' : "" ?> value="t" /> <label for="prsativo-t" style="cursor:pointer;"> Sim</label>
			<input type="radio" name="prsativo" id="prsativo-f" <?php echo ($prsativo == '0' || empty($prsativo) ) ? ' checked="checked" ' : "" ?> value="f" /> <label for="prsativo-f" style="cursor:pointer;"> Não</label>
        	<?php echo obrigatorio(); ?>
        </td>
    </tr>
    <tr>
        <td align='right' class="SubTituloDireita">Exercício Aberto:</td>
        <td>
			<input type="radio" name="prsexercicioaberto" id="prsexercicioaberto-t" <?php echo $prsexercicioaberto == 't' ? ' checked="checked" ' : "" ?> value="t" /> <label for="prsexercicioaberto-t" style="cursor:pointer;"> Sim</label>
			<input type="radio" name="prsexercicioaberto" id="prsexercicioaberto-f" <?php echo ($prsexercicioaberto == 'f' || empty($prsexercicioaberto) ) ? ' checked="checked" ' : "" ?> value="f" /> <label for="prsexercicioaberto-f" style="cursor:pointer;"> Não</label>
        	<?php echo obrigatorio(); ?>
        </td>
    </tr>
    <tr style="background-color: #cccccc">
    <td align='right' style="vertical-align:top; width:25%"> </td>
    <td>
        <input type="button" name="salvar" value="Salvar" onclick="validar_cadastro();"/>
        <input type="button" name="Voltar" value="Voltar" onclick="javascript:location.href='?modulo=sistema/tabapoio/programacaoExercicio&acao=A'"/>
    </td>
    </tr>
</table>
</form>


<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script language="javascript" type="text/javascript" >

function validar_cadastro(cod){
    if (!validaBranco(document.formulario.prsano, 'Ano')) return;
    if (!validaBranco(document.formulario.prsdata_inicial, 'Data de Início')) return;
    if (!validaBranco(document.formulario.prsdata_termino, 'Data de Término')) return;
    if (!validaBranco(document.formulario.prsdtplanejamentoinicio, 'Data de Início do Planejamento')) return;
    if (!validaBranco(document.formulario.prsdtplanejamentotermino, 'Data de Término do Planejamento')) return;

    document.formulario.act.value = 'salvar';
    document.formulario.submit();
}

</script>

