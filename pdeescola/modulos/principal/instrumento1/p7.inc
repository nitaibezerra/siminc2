<?php
pde_verificaSessao();
/*----------------- Ações - Formulario ---------------------*/ 
function salva(){
	global $db,$pdeid,$totalPost;
//	
//	dbg( $_REQUEST );
//	die();
	if($_POST["submetido"]) {
		if($totalPost > 1){
			$sqlDel = "DELETE FROM pdeescola.nivelmodalidadeensino WHERE pdeid = ".$pdeid;
			$db->executar($sqlDel);
 
			for ($i = 1; $i <= $totalPost + 1; $i++){
				$tmeid = $_REQUEST['nivel'.$i];
				if($tmeid != NULL){
					$sql = "INSERT INTO 
							pdeescola.nivelmodalidadeensino(tmeid, pdeid)						
							VALUES(".$tmeid.",".$pdeid.")";
					 $db->executar($sql);
				}
			}
		}
		
		$epfid = $db->pegaUm("SELECT
								epfid
							  FROM
							  	pdeescola.estruturaperfilfuncionamento
							  WHERE
							  	trim(epfnomearquivo) = 'p7' AND
							  	epfano = ".ANO_EXERCICIO_PDE_ESCOLA);
		
		$existe = $db->pegaUm("SELECT
								pepid
							 FROM
							 	pdeescola.pdeepf
							 WHERE
							 	pdeid = ".$pdeid." AND
							 	epfid = ".$epfid);
		
		if($existe == NULL) {
			$sql = "INSERT INTO pdeescola.pdeepf(pdeid,epfid) VALUES(".$pdeid.",".$epfid.")";
			$db->executar($sql);
		}
		 
		$existe_preenchimento = $db->pegaUm("SELECT 
												pprid
											 FROM 
											 	pdeescola.pdepreenchimento
											 WHERE
											 	pdeid = '$pdeid'
											 AND
											 	ppritem = 'p7'");
		if( $existe_preenchimento == NULL )
		{
			$sql_p = "INSERT INTO pdeescola.pdepreenchimento( pdeid, pprinstrumento, ppritem, pdeanoreferencia) VALUES ( '$pdeid', 'i1', 'p7','{$_SESSION['exercicio_atual']}')";
			$db->executar( $sql_p ); 
		}
		$db->commit(); 
 
	}
}

/*----------------- Configurações de Página ---------------------*/ 
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

$pdeid 		= $_SESSION["pdeid"];
$entid  	= $_SESSION["entid"];
$totalPost 	= $_POST['total'];

if (!$pdeid){
	die('<script>
			alert(\'Falta parametros!\nTente novamente.\');
			history.go(-1);
		 </script>');
}

/***********************************************************
 * Componente que irá montar os butões do formulário
 * $cDado = array("instrumento" => 1) ==>> Paramentros que serão carregados pelo 
 * 										   __construct "Padrões à classe".
 * $cForm = new formulario($cDado)    ==>> Instanciando a classe, passando os parametros, 
 * 										   não precisa dar include do arquivo na página.
 * $cForm->direcPag('salvaP13( epaid )', array("epaid" => $epaid)); ==>> Faz o gerenciamento dos métodos 
 * 																		1º parametro
 * 																		passa a função que será aplicada nos botões 
 * 																		"anterior, próximo, salvar anterior, salvar próximo e salvar".
 * 																		2º parametro
 * 																		será um array de "índice" igual ao parametro da funcão 
 * 																		passada no 1º parametro e o valor será o desejado.
 * $cForm->montarbuttons("validaForm()") ==>> Localiza-se no final da tabela do formulário "dentro da tabela ainda", este monta os butões.
 * 											  O parametro passado refere-se a função javascript que validará o formulário,
 * 											  devendo esta submeter o formulário. 
 * 
 ***********************************************************/

$cDado = array("instrumento" => 1);
$cForm = new formulario($cDado);
$cForm->direcPag('salva()');

include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
monta_titulo('Instrumento 1', '');
echo cabecalhoPDE(); 

/*----------------- Recupera dados ---------------------*/  
$dados = $db->carregar("SELECT t.tmeid as id, n.tmeid as selecionado,
								tmedescricao as descricao
						FROM pdeescola.tiponivelmodalidadeensino t
						left join pdeescola.nivelmodalidadeensino n on n.tmeid = t.tmeid and pdeid = ".$pdeid);
$total = count($dados); 
?>
<?=subCabecalho('7 - Nível e modalidade de ensino ministrados na escola.'); ?>	
<form method="post" id="formulario" name="formulario">
<input type="hidden" id="submetido" name="submetido" value="0">
<input type="hidden" name="total" value="<?=$total?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
<? foreach($dados as $dados){ 
	$check = '';
	if($dados['selecionado']){
		$check = 'checked="checked"';
	}
?>
  <tr>
    <td width="28%" class="SubTituloDireita" ><?=$dados['descricao'];?></td>
    <td ><input type="checkbox" id="nivel<?=$dados['id'];?>" name="nivel<?=$dados['id'];?>" value="<?=$dados['id']; ?>" <?=$check;?> /></td>
  </tr>
<? } ?>
  <? $cForm->montarbuttons("submeterDados()");?>				
</table>
</form>
<script language="javascript" type="text/javascript">
function retorna() {
	window.location = "?modulo=principal/estrutura_avaliacao&acao=A&entid=<?=$entid?>";
}

function proximoFormulario() {
	window.location = "?modulo=principal/instrumento1/f3_dependencias&acao=A&pdeid=<?=$pdeid?>&entid=<?=$entid?>";
}

function submeterDados(){
	var validacao1 = false; 
	var validacao2 = false; 
	var validacao3 = false;
	var algummarcado = false; 
	var cont = 1;
	for (i=0;i<document.formulario.elements.length;i++){ 
		if(document.formulario.elements[i].type == "checkbox"){
      		checkBox = document.formulario.elements[i];
      		if ( checkBox.checked == true) {  
      			if((checkBox.id == 'nivel2') ){
      				validacao1 = true;
      			}  
      			if((checkBox.id == 'nivel3') ){
      				validacao2 = true;
      			} 
      			if((checkBox.id == 'nivel4') ){
      				validacao3 = true;
      			} 
		      	algummarcado = true; 
			 }
		}
	}
	if(algummarcado == false){
		alert("Obrigatório selecionar pelo menos um nível de modalidade.");
		return false;
	}

		document.getElementById('submetido').value = 1;
		document.getElementById('formulario').submit();
		return true;
}
//alert(document.formulario.elements[i].checked);
</script>
