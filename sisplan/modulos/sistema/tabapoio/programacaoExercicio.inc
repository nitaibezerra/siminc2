<?php
$prsano = $_REQUEST['prsano_id'];
$programacaoExercicio = new ProgramacaoExercicio( $prsano );

$where = array();
if($_POST['act'] == 'pesquisar'){
    if($_POST['prsano'])
        $where[] = " prs.prsano = '{$_POST['prsano']}' ";
    if($_POST['prsdata_inicial'])
        $where[] = " prs.prsdata_inicial = '".formata_data_sql($_POST['prsdata_inicial'])."' ";
    if($_POST['prsdata_termino'])
        $where[] = " prs.prsdata_termino = '".formata_data_sql($_POST['prsdata_termino'])."' ";
    if($_POST['prsdtplanejamentoinicio'])
        $where[] = " prs.prsdtplanejamentoinicio = '".formata_data_sql($_POST['prsdtplanejamentoinicio'])."' ";
    if($_POST['prsdtplanejamentotermino'])
        $where[] = " prs.prsdtplanejamentotermino = '".formata_data_sql($_POST['prsdtplanejamentotermino'])."' ";
    if($_POST['prsexerccorrente'] != 'a')
        $where[] = " prs.prsexerccorrente = '{$_POST['prsexerccorrente']}' ";
    if($_POST['prsativo'] != 'a'){
    	if($_POST['prsativo'] == 't')
        	$where[] = " prs.prsativo = '1' ";
    	if($_POST['prsativo'] == 'f')
        	$where[] = " prs.prsativo = '0' ";
    }
    if($_POST['prsexercicioaberto'] != 'a')
        $where[] = " prs.prsexercicioaberto = '{$_POST['prsexercicioaberto']}' ";
    extract($_POST);
}

if ($_POST['act'] == 'excluir'){
    $dado = array('prsstatus' => 'I');
    $programacaoExercicio->popularDadosObjeto($dado);
    $programacaoExercicio->salvar();
    $programacaoExercicio->commit();

    die("<script>
            alert('Operação realizada com sucesso!');
            location.href = '?modulo=sistema/tabapoio/programacaoExercicio&acao=A';
        </script>");
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo( $titulo_modulo, '' );
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
                 echo campo_texto('prsano','N','','',11,4,'####','');
             ?>
         </td>
     </tr>
     <tr>
        <td align='right' class="SubTituloDireita">Data de Início:</td>
        <td>
        <?
            echo campo_data2('prsdata_inicial', 'N', 'S', '', 'S');
        ?>
        </td>
    </tr>
     <tr>
        <td align='right' class="SubTituloDireita">Data de Término:</td>
        <td>
        <?
            echo campo_data2('prsdata_termino', 'N', 'S', '', 'S');
        ?>
        </td>
    </tr>
     <tr>
        <td align='right' class="SubTituloDireita">Data de Início do Planejamento:</td>
        <td>
        <?
            echo campo_data2('prsdtplanejamentoinicio', 'N', 'S', '', 'S');
        ?>
        </td>
    </tr>
     <tr>
        <td align='right' class="SubTituloDireita">Data de Término do Planejamento:</td>
        <td>
        <?
            echo campo_data2('prsdtplanejamentotermino', 'N', 'S', '', 'S');
        ?>
        </td>
    </tr>
    <tr>
        <td align='right' class="SubTituloDireita">Exercício Corrente:</td>
        <td>
			<input type="radio" name="prsexerccorrente" id="prsexerccorrente-t" <?php echo $prsexerccorrente == 't' ? ' checked="checked" ' : "" ?> value="t" /> <label for="prsexerccorrente-t" style="cursor:pointer;"> Sim</label>
			<input type="radio" name="prsexerccorrente" id="prsexerccorrente-f" <?php echo $prsexerccorrente == 'f' ? ' checked="checked" ' : "" ?> value="f" /> <label for="prsexerccorrente-f" style="cursor:pointer;"> Não</label>
			<input type="radio" name="prsexerccorrente" id="prsexerccorrente-a" <?php echo ($prsexerccorrente == 'a' || empty($prsexerccorrente) ) ? ' checked="checked" ' : "" ?> value="a" /> <label for="prsexerccorrente-a" style="cursor:pointer;"> Todos</label>
        </td>
    </tr>
    <tr>
        <td align='right' class="SubTituloDireita">Exercício Ativo:</td>
        <td>
			<input type="radio" name="prsativo" id="prsativo-t" <?php echo $prsativo == 't' ? ' checked="checked" ' : "" ?> value="t" /> <label for="prsativo-t" style="cursor:pointer;"> Sim</label>
			<input type="radio" name="prsativo" id="prsativo-f" <?php echo $prsativo == 'f' ? ' checked="checked" ' : "" ?> value="f" /> <label for="prsativo-f" style="cursor:pointer;"> Não</label>
			<input type="radio" name="prsativo" id="prsativo-a" <?php echo ($prsativo == 'a' || empty($prsativo) ) ? ' checked="checked" ' : "" ?> value="a" /> <label for="prsativo-a" style="cursor:pointer;"> Todos</label>
        </td>
    </tr>
    <tr>
        <td align='right' class="SubTituloDireita">Exercício Aberto:</td>
        <td>
			<input type="radio" name="prsexercicioaberto" id="prsexercicioaberto-t" <?php echo $prsexercicioaberto == 't' ? ' checked="checked" ' : "" ?> value="t" /> <label for="prsexercicioaberto-t" style="cursor:pointer;"> Sim</label>
			<input type="radio" name="prsexercicioaberto" id="prsexercicioaberto-f" <?php echo $prsexercicioaberto == 'f' ? ' checked="checked" ' : "" ?> value="f" /> <label for="prsexercicioaberto-f" style="cursor:pointer;"> Não</label>
			<input type="radio" name="prsexercicioaberto" id="prsexercicioaberto-a" <?php echo ($prsexercicioaberto == 'a' || empty($prsexercicioaberto) ) ? ' checked="checked" ' : "" ?> value="a" /> <label for="prsexercicioaberto-a" style="cursor:pointer;"> Todos</label>
        </td>
    </tr>
    <tr style="background-color: #cccccc">
    <td align='right' style="vertical-align:top; width:25%"> </td>
    <td>
        <input type="button" name="Pesquisar" value="Pesquisar" onclick="pesquisar();"/>
        <input type="button" name="Limpar" value="Limpar" onclick="javascript:location.href='?modulo=sistema/tabapoio/programacaoExercicio&acao=A'"/>
    </td>
    </tr>
	<tr>
		<td colspan="2">
			<a style="cursor: pointer;" onclick="javascript: window.location='?modulo=sistema/tabapoio/cadProgramacaoExercicio&acao=A';" title="Clique para incluir uma nova Programação Exercício no sistema">
				<img src="../imagens/incluir.png" style="width: 15px; vertical-align: middle;"/> <b>Incluir nova Programação Exercício</b>
			</a>
		</td>
	</tr>
</table>
</form>
<?php


    $arConfig = array("style"           => "width:95%;",
                      "totalLinha"      => false,
                      "totalRegistro"   => true);
    $arCabecalho = array("Ação", "Ano", "Data de Início","Data de Término","Data de Início do Planejamento","Data de Término do Planejamento","Exercício Corrente","Exercício Ativo","Exercício em Aberto");

    $acao = '<center>
    			<img src=\'/imagens/alterar.gif\' style="cursor:pointer" title="Alterar" onclick="editar( {prsano} )">&nbsp;
            	<img src=\'/imagens/excluir.gif\' style="cursor:pointer" title="Excluir" onclick="excluir( {prsano} )">
             </center>';

    $rs = $programacaoExercicio->getLista( $where, true );

    $oLista = new Lista($arConfig);
    $oLista->setCabecalho( $arCabecalho );
    $oLista->setCorpo( $rs );
    $oLista->setAcao( $acao );
    $oLista->show();
?>


<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script language="javascript" type="text/javascript" >

function pesquisar(cod){
    document.formulario.act.value = 'pesquisar';
    document.formulario.submit();
}

function editar(cod){
	window.location = '?modulo=sistema/tabapoio/cadProgramacaoExercicio&acao=A&prsano_id='+cod;
}

function excluir(cod){
	if ( confirm('Tem certeza que deseja apagar este registro?') ){
        document.formulario.act.value = 'excluir';
        document.formulario.prsano_id.value = cod;

        document.formulario.submit();
	}
}

</script>

