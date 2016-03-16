<?php
pde_verificaSessao();
/*----------------- Configurações de Página ---------------------*/ 
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");


/*----------------- Ações - Formulario ---------------------*/ 
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

function salva(){
	global $db,$pdeid,$totalPost;
	
	if($_POST["submetido"]) {
		if($totalPost > 1){
			$sqlDel = "DELETE FROM pdeescola.dependenciascondicaouso WHERE pdeid = ".$pdeid;
			$db->executar($sqlDel);
			for ($i = 1; $i <= $totalPost; $i++){
				$insert = ""; 
				$valor ="";
				$tidid					= $_REQUEST["dep".$i];
				$quantidade				= $_REQUEST["quantidade".$i];
				$adequado				= $_REQUEST["adequado".$i];
				$inadequado				= $_REQUEST["inadequado".$i];
				$dcudescinadequado		= $_REQUEST["dcudescinadequado".$i];
				if($dcudescinadequado){
					$insert = ",dcudescinadequado"; $valor =",'".substr($dcudescinadequado, 0, 150 )."'";
				}
				
				/*
				 * 
				 * ($_REQUEST["adequado".$i])?($_REQUEST["adequado".$i]): 0
				 * 
				 * 
				 */
				
				
				if($tidid && $quantidade && $adequado != "" && $inadequado != "" ){
						$sql = "INSERT INTO 
							pdeescola.dependenciascondicaouso(	pdeid, 
																tidid,
																dcuqtd,
																dcuqtdcondadequada,
																dcuqtdcondinadequada
																".$insert."
																)						
							VALUES(	".$pdeid.",
									".$tidid.",
									".$quantidade.",
									".substr($adequado, 0, 3).",
									".substr($inadequado, 0, 3)."
									".$valor."
									)";
					$db->executar($sql);
				}
			}
			
			$epfid = $db->pegaUm("SELECT
									epfid
								  FROM
								  	pdeescola.estruturaperfilfuncionamento
								  WHERE
								  	trim(epfnomearquivo) = 'p8' AND
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
												 	ppritem = 'p8'");
			if( $existe_preenchimento == NULL )
			{
				$sql_p = "INSERT INTO pdeescola.pdepreenchimento( pdeid, pprinstrumento, ppritem, pdeanoreferencia) VALUES ( '$pdeid', 'i1', 'p8','{$_SESSION['exercicio_atual']}')";
				$db->executar( $sql_p ); 
			}
			$db->commit();
		}
	}
}	
/*----------------- Recupera dados ---------------------*/  	
 $sqlCombo = "select
                        td.tidid as codigo,
                        td.tiddescricao as descricao,
                        dc.tidid as selecionado,
                        dc.dcuqtd as quantidade,
                        dc.dcuqtdcondadequada as adequado,
                        dc.dcuqtdcondinadequada as inadequado,
                        dc.dcuid,
                        dc.dcudescinadequado  
                    from pdeescola.tipodependencia td
                    left join pdeescola.dependenciascondicaouso dc on dc.tidid = td.tidid and dc.pdeid = ".$pdeid."
                    order by
                        td.tiddescricao
             ";
 $dependencias = $db->carregar( $sqlCombo );
 $total = count($dependencias);	

/*----------------- Monta Cabeçalho ---------------------*/   
include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
monta_titulo('Instrumento 1', ''); 
echo cabecalhoPDE(); 
?>
<?=subCabecalho('8 - Dependência escolares e condições de uso.'); ?>	
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<form id="formulario" name="formulario" method="post" action="">
<input type="hidden" id="submetido" name="submetido" value="0">
<input type="hidden" name="total" value="<?=$total?>">
<table class="tabela listagem" bgcolor="#f5f5f5" align="center">
  <tr>
    <td class="SubtituloEsquerda"  ></td>
    <td class="SubtituloEsquerda"  width="79">Depend&ecirc;ncias</td>
    <td class="SubtituloEsquerda"  width="96">Quantidade</td>
    <td class="SubtituloEsquerda"  colspan="2">Condi&ccedil;&otilde;es de utiliza&ccedil;&atilde;o </td>
    <td class="SubtituloEsquerda">O que está inadequado</td>
  </tr>
  <tr>
  <td class="SubtituloEsquerda"  ></td>
    <td class="SubtituloEsquerda"  colspan="2">&nbsp;</td>
    <td class="SubtituloEsquerda"  width="80">Adequado</td>
    <td class="SubtituloEsquerda"  width="80">Inadequado</td>
    <td class="SubtituloEsquerda"></td>
  </tr>
   <?
   		$podeeditar = 0;
      	foreach($dependencias as $dependencias){
			$ID	  				= $dependencias['codigo'];
			$Nome 				= $dependencias['descricao'];
			$marcado 			= $dependencias['selecionado'];
			$quantidade 		= $dependencias['quantidade'];
			$adequado 			= $dependencias['adequado'];
			$inadequado 		= $dependencias['inadequado'];
			$dcuid	    		= $dependencias['dcuid'];
			$dcudescinadequado 	= $dependencias['dcudescinadequado'];
			$check = '';
      		if($marcado) {
	      		$check = 'checked="checked"';	
	      		$visivel = " display:block; ";
	      		$qtdDisabled = "";
      			$condicoesDisabled = "";
      		}
      		else {
      			$visivel = " display:none;  ";
      			$qtdDisabled = "disabled=\"disabled\"";
      			$condicoesDisabled = "disabled=\"disabled\"";
      		}
   ?>
  <tr>
  	<td>
  	<input type="checkbox" id="dep<?=$ID;?>" name="dep<?=$ID;?>" title="<?= $Nome?>" value="<?=$ID;?>" onclick="criadescricao(<?=$ID;?>);" <?=$check;?> />
  	<input type="hidden" id="dcudescinadequado<?=$ID?>" name="dcudescinadequado<?=$ID?>" value="<?=$dcudescinadequado?>" maxlength="150">
  	</td>
    <td><?=$Nome; ?></td>
    <td> <input style="width:50;" name="quantidade<?=$ID;?>" id="quantidade<?=$ID;?>" value="<?=$quantidade; ?>" maxlength="3" type="text" <?=$qtdDisabled?> onkeypress="return somenteNumeros(event);"  onkeyup="valor_total(<?=$ID;?>);" readonly="true" class="disabled" /></td>
    <td> <input style="width:50" name="adequado<?=$ID;?>" id="adequado<?=$ID;?>" value="<?=$adequado; ?>" maxlength="3" type="text" <?=$condicoesDisabled?> onkeypress="return somenteNumeros(event);" onkeyup="valor_total(<?=$ID;?>);" onblur="coloca_zero(<?=$ID;?>);" /> </td>
    <td> <input style="width:50" name="inadequado<?=$ID;?>" id="inadequado<?=$ID;?>" value="<?=$inadequado; ?>" maxlength="3" type="text" <?=$condicoesDisabled?> onkeypress="return somenteNumeros(event);" onkeyup="valor_total(<?=$ID;?>);"onchange="coloca_zero(<?=$ID;?>);" /></td>
    <td>  
    <span id="descri<?=$ID;?>" style=" <?=$visivel; ?>  cursor:pointer;float:left" onclick="abrepopup( <?= $dcuid = ($dcuid == NULL) ? 0 : $dcuid;?>,<?= $ID = ($ID == NULL) ? 0 : $ID;?> );"><img src="/imagens/gif_inclui.gif" align="absmiddle" style="border: none" /> Editar / Inserir Inadequado</span>
    </td>
  </tr>
  <?
	}
  ?>
  <? $cForm->montarbuttons("submeterDados()");?>				
</table>
</form>
<script language="javascript" type="text/javascript">
function submeterDados() {
		
	document.getElementById('submetido').value = 1;
	document.getElementById('formulario').submit();
	return true;

}
function valor_total(id){

	var total = Number(document.getElementById('adequado'+ id).value) + Number(document.getElementById('inadequado'+id).value);
	document.getElementById('quantidade'+ id).value = total;
}

function coloca_zero(id){
	var adq = document.getElementById('adequado' + id);
	var inadq = document.getElementById('inadequado' + id);
	
	if(adq.value == ""){
		adq.value = 0;
	}
	if(inadq.value == ""){
		inadq.value = 0;
		}

}

function verificaQuantidade(id,tipo) {
	var qtd = document.getElementById('quantidade' + id);
	var adeq = document.getElementById('adequado' + id);
	var inadeq = document.getElementById('inadequado' + id);
		
	var qtdnum = document.formulario.quantidade.value.length ;
	var adeqnum = document.formulario.adequado.value.length ;
	var inadeqnum = document.formulario.inadequado.value.length ;
	
	if(( adeqnum >= 3 ) || ( inadeqnum >= 3 )) {
		alert('Limite máximo de 3 caracteres.');
		return false;
	}	
		 	
	if(qtd.value == "") {
		alert("A quantidade deve ser informada.");
		qtd.focus();
	}
	else {
		if(Number(adeq.value) > Number(qtd.value)) {
			alert("O valor deste campo não pode exceder o do campo 'Quantidade'.");
			adeq.value = "";
		}
		else if(Number(inadeq.value) > Number(qtd.value)) {
			alert("O valor deste campo não pode exceder o do campo 'Quantidade'.");
			inadeq.value = "";
		}
		else if((Number(adeq.value) + Number(inadeq.value)) > Number(qtd.value)) {
			alert("A soma dos campos 'Adequado' e 'Inadequado' não pode ser superior à quantidade determinada");
			if(tipo == "adequado")
				adeq.value = "";
			
			else
				inadeq.value = "";
		}
		
	}
}

function verificaQuantidade2(id,tipo) {
	var qtd = document.getElementById('quantidade' + id);
	var adeq = document.getElementById('adequado' + id);
	var inadeq = document.getElementById('inadequado' + id);
		
		if((adeq.value != "" && inadeq.value != "") && (Number(adeq.value) + Number(inadeq.value)) < Number(qtd.value)) {
			alert("A soma dos campos 'Adequado' e 'Inadequado' não pode ser inferior à quantidade determinada");
			if(tipo == "adequada")
				adeq.value = "";
			else
				inadeq.value = "";
		}
}


function abrepopup(dcuid, ID){
	desc = document.getElementById('dcudescinadequado'+ID).value;
	window.open('http://<?=$_SERVER['SERVER_NAME']?>/pdeescola/combo_dependencias.php?dcuid='+dcuid+'&ID='+ID+'&desc='+desc,'Dependências','width=500,height=350,scrollbars=1');
}

function anteriorFormulario() {
	window.location = "?modulo=principal/instrumento1/f2_nivel_modalidade&acao=A&pdeid=<?=$pdeid?>&entid=<?=$entid?>";
}

function proximoFormulario() {
	window.location = "?modulo=principal/instrumento1/f4_dependencias&acao=A&pdeid=<?=$pdeid?>&entid=<?=$entid?>";
}
function retorna() {
	window.location = "?modulo=principal/estrutura_avaliacao&acao=A&entid=<?=$entid?>";
}

function criadescricao(linha){
	checkBox = document.getElementById("dep"+linha).checked;
	if(checkBox == true){
		document.getElementById('descri'+linha).style.display='block';
		
		document.getElementById('quantidade'+linha).disabled = false;
		document.getElementById('adequado'+linha).disabled = false;
		document.getElementById('inadequado'+linha).disabled = false;
	}else{
		document.getElementById('descri'+linha).style.display='none';
		
		document.getElementById('dcudescinadequado'+linha).value = "";
		
		document.getElementById('quantidade'+linha).disabled = true;
		document.getElementById('quantidade'+linha).value = "";
		document.getElementById('adequado'+linha).disabled = true;
		document.getElementById('adequado'+linha).value = "";
		document.getElementById('inadequado'+linha).disabled = true;
		document.getElementById('inadequado'+linha).value = "";
	}
}
</script>