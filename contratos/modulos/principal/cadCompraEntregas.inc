<?php
$sql = "select docid from evento.coadesao where copid = ". $_SESSION['copid'] ." and usgid = ". $_SESSION['unidade'];
$docid = $db->pegaUm($sql);
if($docid){
	$sql = "select esdid from workflow.documento where docid = $docid";
	$esdid = $db->pegaUm($sql);
} else {
	echo '<script>alert("Cadastro de previsão de entrega somente\npara demandas com adesão aprovada.");</script>';
	echo '<script>javascript:history.back(1);</script>';
}

$coaid = pegaCoaid($_SESSION['copid'], $_SESSION['unidade']);

if($coaid){
	$coaid = $coaid;	
} else {
	echo '<script>alert("Cadastro de previsão de entrega somente\npara demandas com adesão aprovada.");</script>';
	echo '<script>javascript:history.back(1);</script>';
}
if($_POST['completaQuant']){
	extract($_POST);
	$coaid = pegaCoaid($_SESSION['copid'], $_SESSION['unidade']);
	$sql = "SELECT cotid FROM evento.coitemprocesso WHERE coiid = $coiid";
	$cotid = $db->pegaUm($sql);
	$sql = "SELECT cdiqtde FROM evento.codemandaitem WHERE cotid = $cotid AND coaid = $coaid";
	$cdiqtde = $db->pegaUm($sql);
	echo $cdiqtde;
	die;
}
error_reporting(0);
ini_set('display_errors', FALSE); 
$boAlterar = true;
/*
if(!$_SESSION['coaid']){
	echo "<script>
			alert('Favor Cadastrar Endereço');
			window.location.href = '/evento/evento.php?modulo=principal/cadCompraEnd&acao=A';
		  </script>";
	die;
}
*/


verificaSessao(true);
$copid = $_SESSION['copid'];

$sql = "select 
			coeid
	     FROM evento.coenderecoentrega  ee 
	     	inner join evento.coadesao a on a.coaid = ee.coaid
	     where a.copid = ". $_SESSION['copid'] ." and a.usgid = ". $_SESSION['unidade'];
$boEndereco = $db->pegaUm($sql);


if($boEndereco){
	$coaid = pegaCoaid($_SESSION['copid'], $_SESSION['unidade']);
	$boAlterar = permissaoAlterar($coaid);
}

$sql = "SELECT usgid FROM evento.coadesao WHERE coaid = $coaid";
$usgid = $db->pegaUm($sql);

if($_REQUEST["alteraForm"])
{
	$cpiid = $_REQUEST["cpiid"];
	$coiid = $_REQUEST["coiid"];
	$sql = "SELECT pr.coiid, pr.cpipritrimestre, pr.cpisegtrimestre, pr.cpitertrimestre, pr.cpiquatrimestre, c.coidsc FROM evento.coprevisaoentregaitem pr INNER JOIN evento.coitem AS c on c.coiid = pr.coiid WHERE pr.cpiid = $cpiid";
	$dados = $db->pegalinha($sql);
	$sql = "SELECT cotid FROM evento.coitemprocesso WHERE coiid = $coiid";
	$cotid = $db->pegaUm($sql);
	$coaid = pegaCoaid($_SESSION['copid'], $_SESSION['unidade']);
	$sql = "SELECT cdiqtde FROM evento.codemandaitem WHERE cotid = $cotid AND coaid = $coaid";
	$cdiqtde = $db->pegaUm($sql);
	$dados['coiid'] = "<select disabled='disabled'><option>Selecione um item de compra..............</option><option selected >".$dados['coidsc']."</option></select><input type='hidden' name='item' id='item' value='".$dados['coiid']."'>";
	echo $dados['coiid']."|".$dados['cpipritrimestre']."|".$dados['cpisegtrimestre']."|".$dados['cpitertrimestre']."|".$dados['cpiquatrimestre']."|".$cdiqtde;
	exit;
}

if($_REQUEST["excluiForm"])
{
	$cpiid = $_REQUEST["cpiid"];
	//remover DELETE e mudar pra UPDATE (status = 'I')
	$sql = "UPDATE evento.coprevisaoentregaitem SET cpistatus = 'I' WHERE cpiid = $cpiid";
	//$sql = "DELETE FROM evento.coprevisaoentregaitem WHERE cpiid = $cpiid";
	$db->executar( $sql );
	$db->commit();
	exit;
}


if($_POST['submeter'] == 'true'){
	$item = $_POST['item'];
	$per01 = $_POST['per01'];
	$per02 = $_POST['per02'];
	$per03 = $_POST['per03'];
	$per04 = $_POST['per04'];
	$coaid = $_SESSION['coaid'];
	$id = $_POST["cpiid"];
	if($per01 <> ''){
		$per01 = $per01;
	} else {
		$per01 = 0;
	}
	if($per02 <> ''){
		$per02 = $per02;
	} else {
		$per02 = 0;
	}
	if($per03 <> ''){
		$per03 = $per03;
	} else {
		$per03 = 0;
	}
	if($per04 <> ''){
		$per04 = $per04;
	} else {
		$per04 = 0;
	}
	if($id <> ''){
		$sql = "UPDATE evento.coprevisaoentregaitem set	cpipritrimestre = $per01 , cpisegtrimestre = $per02, cpitertrimestre = $per03, 
					cpiquatrimestre = $per04 where cpiid=$id";
	} else {
		$sql = "SELECT cdiid FROM evento.codemandaitem WHERE coaid = $coaid";
		$cdiid = $db->pegaUm($sql);
		
		$sql = "INSERT INTO evento.coprevisaoentregaitem (cdiid,coiid,cpiano,cpipritrimestre,cpisegtrimestre,cpitertrimestre,cpiquatrimestre,cpistatus,usgid,copid) 
							values ($cdiid,$item,'2010',$per01,$per02,$per03,$per04,'A',$usgid,$copid)";	
	}
	$db->executar($sql);	
	$db->commit();
		
	echo "<script>alert('Dados salvos com sucesso');</script>";
}

if( $_SESSION['copid'] != ''){ 
	$sql = "SELECT 
				co.copdsc,  
				co.copnumprocesso,
				to_char(co.copdataabertura::date,'DD/MM/YYYY') AS copdataabertura,		 
				co.cooid, 		 
				co.cocid, 		 
				co.conid, 
				to_char(co.copdatalimite::date,'DD/MM/YYYY') AS copdatalimite,	
				co.usucpf ,
				us.usunome
			FROM 
				evento.coprocesso AS co 
			LEFT JOIN seguranca.usuario AS us ON us.usucpf = co.usucpf 
			WHERE
				co.copid = '".$_SESSION['copid']."'
				";
	
	$rsDadosProcesso = $db->carregar( $sql );
}
//Chamada de programa
if($_GET['boExibeCabecalho'] != 'N'){
	include  APPRAIZ."includes/cabecalho.inc";
	echo'<br>';
	$db->cria_aba( $abacod_tela, $url, '' );
	monta_titulo( $titulo_modulo, '' );
	montaCabecalhoUnidade($_SESSION['unidade']); 
	montaCabecalhoProcesso($_SESSION['copid'], false);

		
} else {
	monta_titulo( $titulo_modulo, '' );
?>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>
<?php
}
?>
<script src="../includes/prototype.js"></script>
<body <?php if ($esdid <> 46){?>onload="alert('Cadastro de previsão de entrega somente\npara demandas com adesão aprovada.');"<?php } ?>>
<form name = "formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario">
	<input type="hidden" name="cpiid" id="cpiid">
	<input type="hidden" name="totalQnt" id="totalQnt">
	<input type="hidden" name="submeter" id="submeter">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    		<tr>
    		    <td class ="SubTituloDireita" align="right">Ano: </td>
                <td><input class="normal" type="text" disabled size="11" name="ano" value="2010"></td>
            </tr>
            <tr>
                <td class ="SubTituloDireita" align="right">Item: </td>
                <td id="select">
                <?php
                if($coaid){ 
					$sql = "SELECT DISTINCT
											c.coiid as codigo,
	        								c.coidsc as descricao
							FROM
											evento.codemandaitem cd
							INNER JOIN evento.coitemprocesso  AS ci ON cd.cotid = ci.cotid
							INNER JOIN evento.coitem AS c on c.coiid = ci.coiid
							WHERE
											cd.coaid = $coaid
											AND c.coiid NOT IN (select coiid from evento.coprevisaoentregaitem where usgid = $usgid AND copid = $copid AND cpistatus='A')";
					if ($esdid <> 46){
	                	$db->monta_combo( 'item', $sql, 'N', 'Selecione um item de compra...', 'completaQuant(this.value)', '', '', '250','N','item','','','Itens' );
					}else{
						$db->monta_combo( 'item', $sql, 'S', 'Selecione um item de compra...', 'completaQuant(this.value)', '', '', '250','N','item','','','Itens' );
					}
                }
                ?>  
				</td>  
            </tr> 
            <tr>
           	<td class ="subTituloDireita" >
           		01º Trimestre:
           	</td>
           	<td>
           		<table width="60%" border='0'> 
	           		<tr>
	           			<td width="25%"><b> Quantidade</b> </td>
	           			<td width="25%"><b> Total</b></td> 
	           			<td width="25%"><b> </b></td>
	           			<td width="25%"><b> </b></td>
	           		</tr> 
	           	   <tr>
	           			<td width="20%"  > 
	           				<input class="normal" type="text" size="11" onkeypress="return somenteNumeros(event);" onchange="subtraiCampos(1);" onfocus="calculaTotal(1)" name="per01" title="1º Trimestre">
	           			</td>
	           			<td id="colunaQtdTotal" width="20%"> 
	           				<input class="normal" disabled type="text" size="11" id="totalPer01">
	               		</td>
	               	</tr>
           		</table> 
        	</td>  
           	</tr>
           	<tr>
	           	<td class ="subTituloDireita" >
	           		02º Trimestre:
	           	</td>
	           	<td>
	           		<table width="60%" border='0'> 
	           			<tr>
	           				<td width="25%"> 
		        				<input class="normal" type="text" size="11" onkeypress="return somenteNumeros(event);" onchange="subtraiCampos(2);" onfocus="calculaTotal(2)" name="per02" title="2º Trimestre">		        			</td>
		        			<td id="colunaQtdTotal" width="25%"> 
		        				<input class="normal" disabled type="text" size="11" id="totalPer02">
		        			</td>
		        			<td width="25%"><b> </b></td>
	           				<td width="25%"><b> </b></td>
		        		</tr>
		        	</table>
           	</tr>
           	<tr>
	           	<td class ="subTituloDireita" >
	           		03º Trimestre:
	           	</td>
	           	<td>
	           		<table width="60%" border='0'> 
	           			<tr>
	           				<td width="25%"> 
		        				<input class="normal" type="text" size="11" onkeypress="return somenteNumeros(event);" onchange="subtraiCampos(3);" onfocus="calculaTotal(3)" name="per03" title="3º Trimestre">
		        			</td>
		        			<td id="colunaQtdTotal" width="25%"> 
		        				<input class="normal" disabled type="text" size="11" id="totalPer03">
		        			</td>
		        			<td width="25%"><b> </b></td>
	           				<td width="25%"><b> </b></td>
		        		</tr>
		        	</table>
           	</tr>
           	<tr>
	           	<td class ="subTituloDireita" >
	           		04º Trimestre:
	           	</td>
	           	<td>
	           		<table width="60%" border='0'> 
	           			<tr>
	           				<td width="25%"> 
		        				<input class="normal" type="text" size="11" onkeypress="return somenteNumeros(event);" onchange="subtraiCampos(4);" onfocus="calculaTotal(4)" name="per04" title="4º Trimestre">
		        			</td>
		        			<td id="colunaQtdTotal" width="25%"> 
		        				<input class="normal" disabled type="text" size="11" id="totalPer04">
		        			</td>
		        			<td width="25%"><b> </b></td>
	           				<td width="25%"><b> </b></td>
		        		</tr>
		        	</table>
           	</tr>
           	<tr style="background-color: #DCDCDC">
				<td  align="center" colspan="2">
					<!--  input type="button" value="Incluir" onclick="validaComponente01();">-->
					<?php if ($esdid == 46){?><input type="button" value="Gravar" onclick="validar()" ><?php } ?>
				</td>
			</tr>	
           
 </table>
 </form>
 <tr>
		<td colspan="2">
		<div  style="display: ''">
			<?php 
				$sql = "SELECT '<center><img src=\"/imagens/alterar.gif \" style=\"cursor: pointer\" onclick=\"alterarFormulario('||coent.cpiid||','||coent.coiid||');\" border=0 alt=\"Ir\" title=\"Alterar\"> </a>' ||
							 '<img src=\"/imagens/excluir.gif \" style=\"cursor: pointer\" onclick=\"excluiFormulario('||coent.cpiid||');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>' as acao,
						 	co.coidsc, coent.cpipritrimestre, coent.cpisegtrimestre, coent.cpitertrimestre, coent.cpiquatrimestre
						FROM 
							evento.coprevisaoentregaitem coent
						
						INNER JOIN evento.coitem co ON co.coiid = coent.coiid
						WHERE
							coent.cpistatus = 'A' AND
							coent.usgid = $usgid AND
							coent.copid = $copid";
				$cabecalho = array("Ações","Item","01º Trimestre", "02º Trimestre", "03º Trimestre", "04º Trimestre");
				$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', '', '');
			?>
		</div>
		</td>
	</tr>
 <script>

function validar(){
	var nomeform      = 'formulario';
    var submeterForm  = true;
    var campos        = new Array();
    var tiposDeCampos = new Array();
    var boVerifica = false;
    var formulario = $('formulario');

    campos[0]	      = "item";
                                    
    tiposDeCampos[0]  = "select";

    for (i=0; i < formulario.length; i++ ){
    	if(formulario.elements[i].name.indexOf('per') != -1 ){
			if(formulario.elements[i].value != ''){
				boVerifica = true;
			}
    	}
    }

    if(boVerifica == true){
        $('submeter').value = 'true';
    	validaForm(nomeform, campos, tiposDeCampos, submeterForm );
    } else {
		alert('Selecione ao menos um trimestre.');
    }
}
 
function alterarFormulario(cpiid, coiid){
		var myAjax = new Ajax.Request('evento.php?modulo=principal/cadCompraEntregas&acao=A', {
							method 	   	 : 'post',
							parameters 	 : '&alteraForm=true&cpiid='+ cpiid +'&coiid='+ coiid,
							asynchronous : false,
							onComplete   : function(res){
								dados = res.responseText;
		   	    			 	dados = dados.split('|');
				       			document.formulario.per01.value = dados[1];
				       			document.formulario.per02.value = dados[2];
				 	      		document.formulario.per03.value = dados[3];
				   	    		document.formulario.per04.value = dados[4];
				   	    		$('select').innerHTML = dados[0];
				   	    		document.formulario.totalQnt.value = dados[5];
				   	    		document.formulario.cpiid.value = cpiid;
				   	    		document.getElementById('item').disabled = true;
				   	    	}
						});
}

function excluiFormulario(cpiid){
	var myAjax = new Ajax.Request('evento.php?modulo=principal/cadCompraEntregas&acao=A', {
						method 	   	 : 'post',
						parameters 	 : '&excluiForm=true&cpiid='+ cpiid,
						asynchronous : false,
						onComplete   : function(res){
							alert('Dados excluídos com sucesso.');
							window.location.href='evento.php?modulo=principal/cadCompraEntregas&acao=A';
						}
					});
}


function completaQuant(coiid){
	if(coiid != ''){
		var data = 'completaQuant=1&coiid='+coiid;
		var aj = new Ajax.Request('evento.php?modulo=principal/cadCompraEntregas&acao=A',  
				{  
					method: 'post',   
					parameters: data,   
					onComplete: function(r)
					{
						if(r.responseText){
							$('totalQnt').value = r.responseText;
						}
					}
				});
	}
}

function subtraiCampos(id){
	valor1 = new Number(document.formulario.per01.value);
	valor2 = new Number(document.formulario.per02.value);
	valor3 = new Number(document.formulario.per03.value);
	valor4 = new Number(document.formulario.per04.value);
	totalQnt = new Number(document.formulario.totalQnt.value);
	if(id==1){
		total = new Number(document.formulario.totalPer01.value);
		if(totalQnt >= (valor1 + valor2 + valor3 + valor4)){
			document.formulario.totalPer01.value = new Number(total - valor1);
		} else {			
			alert('Você não pode escolher '+valor1+' itens desse produto. O máximo que poderá escolher é '+total);
			document.formulario.per01.value = '';
		}
	}
	if(id==2){
		total = new Number(document.formulario.totalPer02.value);
		if(totalQnt >= (valor1 + valor2 + valor3 + valor4)){
			document.formulario.totalPer02.value = new Number(total - valor1);
		} else {
			alert('Você não pode escolher '+valor2+' itens desse produto. O máximo que poderá escolher é '+total);
			document.formulario.per02.value = '';
		}
	}
	if(id==3){
		total = new Number(document.formulario.totalPer03.value);
		if(totalQnt >= (valor1 + valor2 + valor3 + valor4)){
			document.formulario.totalPer03.value = new Number(total - valor1);
		} else {
			alert('Você não pode escolher '+valor3+' itens desse produto. O máximo que poderá escolher é '+total);
			document.formulario.per03.value = '';
		}
	}
	if(id==4){
		total = new Number(document.formulario.totalPer04.value);
		if(totalQnt >= (valor1 + valor2 + valor3 + valor4)){
			document.formulario.totalPer04.value = new Number(total - valor1);
		} else {
			alert('Você não pode escolher '+valor4+' itens desse produto. O máximo que poderá escolher é '+total);
			document.formulario.per04.value = '';
		}
	}
}

function calculaTotal(id){
		valor1 = new Number(document.formulario.per01.value);
		valor2 = new Number(document.formulario.per02.value);
		valor3 = new Number(document.formulario.per03.value);
		valor4 = new Number(document.formulario.per04.value);
		totalQnt = new Number(document.formulario.totalQnt.value);
	if(id==1){
		document.formulario.totalPer01.value = new Number(totalQnt - valor1 - valor2 - valor3 - valor4);
		document.formulario.totalPer02.value = '';
		document.formulario.totalPer03.value = '';
		document.formulario.totalPer04.value = '';
	} 
	if(id==2){
		document.formulario.totalPer02.value = new Number(totalQnt - valor1 - valor2 - valor3 - valor4);
		document.formulario.totalPer01.value = '';
		document.formulario.totalPer03.value = '';
		document.formulario.totalPer04.value = '';
	}
	if(id==3){
		document.formulario.totalPer03.value = new Number(totalQnt - valor1 - valor2 - valor3 - valor4);
		document.formulario.totalPer01.value = '';
		document.formulario.totalPer02.value = '';
		document.formulario.totalPer04.value = '';
	}
	if(id==4){
		document.formulario.totalPer04.value = new Number(totalQnt - valor1 - valor2 - valor3 - valor4);
		document.formulario.totalPer01.value = '';
		document.formulario.totalPer02.value = '';
		document.formulario.totalPer03.value = '';
	} 
}

</script>