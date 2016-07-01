<?php
//Verificando a data de inicio e termino do planejamento ( definida na programacaoexercicio )
$programacaoExercicio = new ProgramacaoExercicio();
$permissao = $programacaoExercicio->verificaPermissaoPlanejamento();

$permissao_formulario = 'S';
$id				= $_REQUEST['exuid'];
$excecaoUnidade	= new ExcecaoUnidade($id);

$where = array();
if($_POST['act'] == 'pesquisar'){
    if($_REQUEST['uexid']){
        $where[] = "uex.uexid = {$_REQUEST['uexid']}";
        $momid   = $_REQUEST['uexid'];
    }

    extract($_POST);
}

if ($_POST['act'] == 'excluir'){
    $arDados = array('exustatus' => 'I');
    $excecaoUnidade->popularDadosObjeto($arDados);
    $excecaoUnidade->salvar();
    $excecaoUnidade->commit();

    die("<script>
            alert('Operação realizada com sucesso!');
            location.href = '?modulo=sistema/tabapoio/excecaoUnidade&acao=A';
        </script>");
}

if( !$db->testa_superuser() ){
	if(is_array( $responsabilidades ) && !empty( $responsabilidades )){
		$cond 		= " uexid IN ('".implode("','", $responsabilidades)."') ";
		$condLista	= " uex.uexid IN ('".implode("','", $responsabilidades)."') ";
	}else{
		$cond = $condLista = " FALSE ";
	}
}else{
	$cond = $condLista = " TRUE ";
}
$where[] = $condLista;

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( $titulo_modulo, '' );
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<form method="POST" name="formulario" action="">
<input type="hidden" name="exuid" value="<?php echo $id ?>"/>
<input type=hidden name="act" value="0"/>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
     <tr>
         <td align='right' class="SubTituloDireita" style="width:25%;">
            Unidade Executora:
        </td>
         <td>
             <?php
                    $modelo = new UnidadeExecutora();
					$dados  = $modelo->recuperarTodos("uexid AS codigo, uexsigla || ' - ' || uexdsc AS descricao", array("uexstatus = 'A'",$cond), "descricao");
                    $db->monta_combo("uexid", $dados, 'S', '&nbsp', '', '', '', '440', 'N', 'uexid');
             ?>
         </td>
     </tr>
    <tr style="background-color: #cccccc">
        <td align='right' style="vertical-align:top; width:25%"> </td>
        <td>
            <input type="button" name="Pesquisar" value="Pesquisar" onclick="pesquisar();"/>
            <input type="button" name="Limpar" value="Limpar" onclick="javascript:location.href='?modulo=sistema/tabapoio/excecaoUnidade&acao=A'"/>
        </td>
    </tr>
	<tr>
		<td colspan="2">
			<a style="cursor: pointer;" onclick="javascript: window.location='?modulo=sistema/tabapoio/cadExcecaoUnidade&acao=A';" title="Clique para incluir nova Exceção para Unidades">
				<img src="../imagens/incluir.png" style="width: 15px; vertical-align: middle;"/> <b>Incluir nova Exceção para Unidades</b>
			</a>
		</td>
	</tr>
</table>
</form>
<?php
    $arConfig = array("style"           => "width:95%;",
                      "totalLinha"      => false,
                      "totalRegistro"   => true);
    $arCabecalho = array("Ação","Unidade", "Data");

    $acao = '<center>
    			<img src=\'/imagens/alterar.gif\' style="cursor:pointer" title="Alterar" onclick="editar( {exuid} )">&nbsp;
            	<img src=\'/imagens/excluir.gif\' style="cursor:pointer" title="Excluir" onclick="excluir( {exuid} )">
             </center>';

    $rs = $excecaoUnidade->getLista( $where, true );

    $oLista = new Lista($arConfig);
    $oLista->setCabecalho( $arCabecalho );
    $oLista->setCorpo( $rs );
    $oLista->setAcao( $acao );
    $oLista->show();
?>


<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
<script language="javascript" type="text/javascript" >

function pesquisar(){
    document.formulario.act.value = 'pesquisar';
    document.formulario.submit();
}

function editar(cod){
	window.location = '?modulo=sistema/tabapoio/cadExcecaoUnidade&acao=A&exuid='+cod;
}

function excluir(cod){
	if ( confirm('Tem certeza que deseja apagar este registro?') ){
        document.formulario.act.value = 'excluir';
        document.formulario.exuid.value = cod;

        document.formulario.submit();
	}
}

</script>

