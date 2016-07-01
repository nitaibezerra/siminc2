<?php 
$indid = $_GET['indid'] ? $_GET['indid'] : $_SESSION['indid'];
$_SESSION['indid'] = $indid;

if(!$_SESSION['indid']){
	header("Location: painel.php?modulo=principal/lista&acao=A");
}

if($_POST){
	//INDICADORES VINCULADOS
	$sql = "UPDATE painel.indicadoresvinculados set idvstatus = 'I' WHERE indidvinculo =  ". $_SESSION['indid'] ."";			
	$db->executar($sql);
	$_POST['indid_vinculado'] = $_POST['indid_vinculado'] ? $_POST['indid_vinculado'] : array();
	foreach($_POST['indid_vinculado'] as $indid_vinculado) {
		if($indid_vinculado){
			$sql = "INSERT INTO painel.indicadoresvinculados(indid, indidvinculo,idvstatus) VALUES (". $indid_vinculado .", ". $_SESSION['indid'] .",'A')";			
			$db->executar($sql);
		}
	}
	$db->commit();
}

// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$db->cria_aba($abacod_tela,$url,'');

$titulo = "Vínculo do Indicador";
monta_titulo( $titulo, "&nbsp;" );
cabecalhoIndicador($_SESSION['indid']);
?>
<style>
.SubtituloTabela{background-color:#cccccc}
.negrito{font-weight:bold}
.bold{font-weight:bold}
.normal{font-weight:normal}
.center{text-align: center;}
.direita{text-align: right;}
.esquerda{text-align: left;}
.msg_erro{color:#990000}
.link{cursor: pointer}
.mini{width:12px;height:12px}
.sucess_msg{color: blue;}
.img_middle{vertical-align:middle}
.hidden{display:none}
.absolute{position:absolute;padding-top:5px;padding-bottom:5px;padding-left:5px;margin-top:5px;margin-left:50px;border:solid 1px black;background-color: #FFFFFF}
.fechar{position:relative;right:-5px;top:-26px;}
.img{background-color:#FFFFFF}
.red{color:#990000}
.mensagem{text-align: center; font-weight: bold;background-color:#cccccc;color: blue;}
.field_ano{width:100%;padding:5px;margin:3px}
</style>
<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-1.7.1.min.js"></script>
<script>
function salvarVinculo()
{
	selectAllOptions( document.getElementById( 'indid_vinculado' ) );
	jQuery("[name='formulario']").submit();
}
</script>
<form name="formulario" method="post" action="" >
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td align='right' class="SubTituloDireita">Indicador(es) Vinculado(s): (Pai)</td>
		<td>
		<?php 
		$Sql = "SELECT 
					indid || ' - ' || indnome AS descricao, 
					indid AS codigo
				FROM 
					painel.indicador
				order by
					 descricao";
		if($indid){
			$SqlCarregado = "SELECT 
								ind.indid || ' - ' || indnome AS descricao, 
								ind.indid AS codigo
							FROM 
								painel.indicador ind
							INNER JOIN
								painel.indicadoresvinculados vin ON vin.indid = ind.indid
							WHERE 
								idvstatus = 'A'
							and
								vin.indidvinculo = {$_SESSION['indid']}
							order by
								 descricao";

			$indid_vinculado = $db->carregar($SqlCarregado);
		}
		combo_popup( 'indid_vinculado', $Sql, 'Selecione o(s) Indicador(es) ', '360x460', 0, array(), "", "S", true, false, 5, 400,"","","",array(
			  array(
			  		"codigo" 	=> "indnome",
			  		"descricao" => "Indicador"
			  		)
			  )	,"",true,true);
		
		//pega permissoes para verificar se habilita o botão
		$permissoes = verificaPerfilPainel();
		
		if($permissoes['verindicadores'] == 'vertodos') {
			$acesso = true;
		} else {
			$acesso = validaAcessoIndicadores($permissoes['verindicadores'], $_SESSION['indid']);
		}
		?>
		</td>
	</tr>
	</tr>
		<td class="SubtituloTabela center" colspan="2"  >
			<input type="button" name="btn_salvar" <? echo $acesso?'':'disabled' ?> value="Salvar" onclick="salvarVinculo()" />
		</td>
	</tr>
</table>
</form>