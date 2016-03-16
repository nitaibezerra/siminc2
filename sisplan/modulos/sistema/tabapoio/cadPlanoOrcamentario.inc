<script type="text/javascript" src="../includes/prototype-1.6.0.3.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="javascript" type="text/javascript" src="../includes/micoxAjax.js"></script>
<script language="javascript" type="text/javascript" >

jQuery.noConflict();

</script>

<?php

$permissao_formulario = 'S';
$id     = $_REQUEST['ploid'];
$modelo = new PlanoOrcamentario($id);
extract($modelo->getDados());

if ($_POST['act'] == 'salvar'){

	//recuperando o $acaid
	if ($_REQUEST['prgcod'] && $_REQUEST['acacod'] && $_REQUEST['unicod'] && $_REQUEST['loccod']){
		$acaid = $db->pegaUm( sprintf( "SELECT acaid FROM monitora.acao WHERE acasnrap = false AND prgano = '%s' AND prgcod = '%s' AND acacod = '%s' AND unicod = '%s' AND loccod = '%s'"
									, $_SESSION['exercicio']
									, $_REQUEST['prgcod']
									, $_REQUEST['acacod']
									, $_REQUEST['unicod']
									, $_REQUEST['loccod']
									)
								);
	}
	if( !$acaid ){
		echo "<script> alert('Não foi possível encontrar a ação atravéz dos parametros informados.'); </script>";
		extract($_REQUEST);
	}else{

	    $arDados = array(
	                        'ploid'     	=> $_REQUEST['ploid'],
	                        'plocod'    	=> $_REQUEST['plocod'],
	                        'plonome'    	=> $_REQUEST['plonome'],
	                        'plodescricao'	=> $_REQUEST['plodescricao'],
	                        'acaid'    		=> $acaid,
	                        'exustatus' 	=> 'A'
	                      );

	    $modelo->popularDadosObjeto($arDados)->salvar();
	    $modelo->commit();
	    $db->sucesso( 'sistema/tabapoio/planoOrcamentario', '');
	}
}

if ($acaid){
	$dados_acaid = $db->pegaLinha( sprintf( "SELECT a.prgcod, a.acacod, a.unicod, a.loccod, u.unmdsc, p.prodsc FROM monitora.acao a LEFT JOIN public.produto
	p ON p.procod = a.procod LEFT JOIN public.unidademedida u ON u.unmcod = a.unmcod
	WHERE acaid = '%s'", $acaid ) );

	extract($dados_acaid);
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$titulo = (empty($id) ? 'Cadastro de Plano Orçamentário' : 'Alteração de Plano Orçamentário');
monta_titulo( $titulo, obrigatorio() . 'Indica Campo Obrigatório.' );
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<form method="POST" name="formulario" action="">
<input type="hidden" name="ploid" value="<?= $ploid; ?>"/>
<input type=hidden name="act" value="0"/>

<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
     <tr>
         <td align='right' class="SubTituloDireita" style="width:25%;">
            Código:
        </td>
         <td>
	        <?
	            echo campo_texto('plocod','S','','',10,4,'','');
	        ?>
         </td>
     </tr>
     <tr>
         <td align='right' class="SubTituloDireita" style="width:25%;">
            Nome:
        </td>
         <td>
	        <?
	            echo campo_texto('plonome','S','','',51,200,'','');
	        ?>
         </td>
     </tr>
     <tr>
         <td align='right' class="SubTituloDireita" style="width:25%;">
            Descrição:
        </td>
         <td>
	        <?
           		echo campo_textarea('plodescricao', 'S', 'S', '', 69, 5, 500 );
	        ?>
         </td>
     </tr>
	<tr>
		<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">PTRES:</td>
		<td>
			<?=campo_texto('ptres','N',$permissao_formulario,'',12,6,'######','','','Informe o número PTRES.','','id="ptres"');?>
			<span id="span_ptres"></span>
			<script type="text/javascript">
			$("ptres").observe("blur", function(event) {
			listar_ptres($("ptres").value);});
			</script>
		</td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Programa:</td>
		<td><?php
			$sql = sprintf( "SELECT distinct p.prgcod as codigo, p.prgcod || ' - ' || prgdsc AS descricao FROM monitora.programa p INNER JOIN monitora.acao a ON a.prgid = p.prgid WHERE a.unicod = '%s' and p.prgano = '%d' AND prgstatus = 'A' ORDER BY 2", UNIDADE_IPHAN, $_SESSION['exercicio'] );
			$db->monta_combo("prgcod",$sql,$permissao_formulario,'&nbsp','listar_acoes','','','570','S','prgcod');
			//echo  obrigatorio();
		?></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Ação:</td>
		<td><?php

			$sql = array();
			if ( usuario_possui_perfil( PERFIL_COORDENADOR_ACAO_SIGPLAN ) )
			{
				$sql = "
                    SELECT
                        ur.acacod
                    FROM sisplan.usuarioresponsabilidade ur
                    WHERE ur.usucpf = '". $_SESSION['usucpf'] . "'
                      AND ur.acacod is not null
                      AND ur.rpustatus = 'A'";

                    $acacodArr = (array) $db->carregarColuna($sql);
                    if ($acacodArr[0]){
                        $acacodWhere = " AND acacod IN ('" . ( implode("','", $acacodArr) ) . "')";
                        $sql = sprintf( "SELECT distinct acacod as codigo, acacod || ' - ' || acadsc AS descricao FROM monitora.acao WHERE acasnrap = false AND prgano = '%d' AND acacod IN ('" . ( implode("','", $acacodArr) ) . "') ORDER BY 2", $_SESSION['exercicio'] );
                    }
			}
			if ( $db->testa_superuser() ||
				 usuario_possui_perfil( PERFIL_APOIO_COORDENADOR_PLANEJAMENTO ) ||
				 usuario_possui_perfil( PERFIL_COORDENADOR_PLANEJAMENTO ) ||
				 usuario_possui_perfil( PERFIL_APOIO_COORDENADOR_ORCAMENTO ) ||
				 usuario_possui_perfil( PERFIL_COORDENADOR_ORCAMENTO ) ) {
				$sql = sprintf( "SELECT distinct acacod as codigo, acacod || ' - ' || acadsc AS descricao FROM monitora.acao a WHERE unicod = '%s' and acasnrap = false AND prgano = '%d' ORDER BY 2", UNIDADE_IPHAN, $_SESSION['exercicio'] );
			}
			$db->monta_combo("acacod",$sql,$permissao_formulario,'&nbsp','listar_unidades','','','570','S','acacod');
			//echo  obrigatorio();
		?><span id="span_acacod"></span></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Unidade Orçamentária:</td>
		<td><?php

			if ( $acacod )
				$sql = sprintf( "select distinct u.unicod as codigo, u.unicod || ' - ' || u.unidsc as descricao from public.unidade u join monitora.acao a on a.unicod = u.unicod join monitora.programa p on a.prgano = p.prgano and p.prgano = '%d' and a.acacod = '%s' order by 2", $_SESSION['exercicio'], $acacod );
			else
				$sql = sprintf( "select distinct u.unicod as codigo, u.unicod || ' - ' || u.unidsc as descricao from public.unidade u join monitora.acao a on a.unicod = u.unicod join monitora.programa p on a.prgano = p.prgano and p.prgano = '%d' order by 2", $_SESSION['exercicio'] );
			$db->monta_combo("unicod",$sql,$permissao_formulario,'&nbsp','listar_localizadores','','','570','S','unicod');
			//echo  obrigatorio();
		?><span id="span_unicod"></span></td>
	</tr>
	<tr>
		<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Localizador:</td>
		<td><?php
					if ( $prgcod && $acacod && $unicod )
					$sql = sprintf( "SELECT DISTINCT a.loccod AS codigo, l.loccod || ' - ' || l.locdsc AS descricao FROM public.localizador l JOIN monitora.acao a ON a.loccod = l.loccod WHERE a.prgano = '%d' AND a.unicod = '%s' AND a.prgcod = '%s' AND a.acacod = '%s' ORDER BY 2", $_SESSION['exercicio'], $unicod, $prgcod, $acacod );
				else
					$sql = "SELECT DISTINCT l.loccod AS codigo, l.loccod || ' - ' || l.locdsc AS descricao FROM public.localizador l JOIN monitora.acao a ON a.loccod = l.loccod ORDER BY 2";
//					$db->monta_combo("loccod",$sql,$permissao_formulario,'&nbsp','listar_produto','','','580','S','loccod');
					$db->monta_combo("loccod",$sql,$permissao_formulario,'&nbsp','','','','570','S','loccod');
			?><span id="span_loccod"></span></td>
	</tr>
    <tr style="background-color: #cccccc">
        <td align='right' style="vertical-align:top; width:25%">
            &nbsp;
        </td>
        <td>
            <input type="button" name="salvar" value="Salvar" onclick="validar_cadastro();"/>
            <input type="button" name="Voltar" value="Voltar" onclick="javascript:location.href='?modulo=sistema/tabapoio/planoOrcamentario&acao=A'"/>
        </td>
    </tr>
</table>
</form>

<script language="javascript" type="text/javascript" >

var p_prgcod = '<?= $prgcod; ?>';
var p_acacod = '<?= $acacod; ?>';
var p_unicod = '<?= $unicod; ?>';
var p_loccod = '<?= $loccod; ?>';
var p_iteid = '<?= $iteid; ?>';
var p_sbcid = '<?= $sbcid; ?>';

function validar_cadastro(){

    if (!validaBranco(document.formulario.plocod, 'Código')) return;
    if (!validaBranco(document.formulario.plonome, 'Nome')) return;
    if (!validaBranco(document.formulario.plodescricao, 'Descrição')) return;
    if (!validaBranco(document.formulario.prgcod, 'Programa')) return;
    if (!validaBranco(document.formulario.acacod, 'Ação')) return;
    if (!validaBranco(document.formulario.unicod, 'Unidade Orçamentária')) return;
    if (!validaBranco(document.formulario.loccod, 'Localizador')) return;

    document.formulario.act.value = 'salvar';
    document.formulario.submit();
}

function listar_ptres(id)
{
	if ( id != '' )
		ajaxGet('../ajax/lista_campos_ptres.php?ptres='+id,document.getElementById('span_ptres'),true);
}

function listar_acoes( prgcod )
{
	var campo_select = document.getElementById( 'acacod' );
	while( campo_select.options.length )
	{
		campo_select.options[0] = null;
	}
	campo_select.options[0] = new Option( '', '', false, true );

	if( prgcod != '' )
	{
		ajaxGet('../ajax/lista_acoes.php?prgcod='+prgcod+'&acacod='+p_acacod,document.getElementById('span_acacod'),true);
	}
}

function listar_unidades( acacod )
{
	var campo_select = document.getElementById( 'unicod' );
	var campo_dep = document.getElementById( 'depid' );

	if( campo_select ){
		while( campo_select.options.length ){
			campo_select.options[0] = null;
		}
	}

	if ( campo_dep ){
		while( campo_dep.options.length ){
			campo_dep.options[0] = null;
		}
	}

	campo_select.options[0] = new Option( '', '', false, true );
	if( acacod != '' )
	{
		ajaxGet('../ajax/lista_unidades.php?acacod='+acacod+'&unicod='+p_unicod,document.getElementById('span_unicod'),true);
		//listagem_produto( acacod );
		//listar_coordenador( acacod );
	}
}

function listar_localizadores( unicod )
{
	var prgcod = document.formulario.prgcod.value && document.formulario.prgcod.value != 'null' ? document.formulario.prgcod.value : p_prgcod;
	var acacod = document.formulario.acacod.value && document.formulario.acacod.value != 'null' ? document.formulario.acacod.value : p_acacod;
	var unicod = document.formulario.unicod.value && document.formulario.unicod.value != 'null' ? document.formulario.unicod.value : p_unicod;

	var campo_select = document.getElementById( 'loccod' );
	while( campo_select.options.length )
	{
		campo_select.options[0] = null;
	}
	campo_select.options[0] = new Option( '', '', false, true );

	if( unicod !='' && prgcod != '' && acacod != '')
	{
		ajaxGet('../ajax/lista_localizadores.php?prgcod='+prgcod+'&acacod='+acacod+'&unicod='+unicod+'&loccod='+p_loccod,document.getElementById('span_loccod'),true);
	}
}

</script>

