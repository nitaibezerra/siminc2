<?php
include_once( APPRAIZ . 'emenda/classes/PTA.class.inc');
$obPTA = new PTA();

//$obPTA->validaSessionPTA( $_REQUEST['ptrid'] );

if($_REQUEST['pesquisa']){
	header('content-type: text/html; charset=ISO-8859-1');
	$obPTA->popupCadastraKit($_POST['pteid']);
	exit;
}

if($_REQUEST['inserekit']){
	header('content-type: text/html; charset=ISO-8859-1');
	$obPTA->insereKitPlanoTrabalho($_POST);
	exit;
}

if($_REQUEST['excluir']){
	header('content-type: text/html; charset=ISO-8859-1');
	$obPTA->excluirKitEspecificacao($_POST['ptkid'], $_POST['pteid']);
	exit;
}

if($_REQUEST['alterar']){
	header('content-type: text/html; charset=ISO-8859-1');
	$obPTA->alterarKitEspecificacao($_POST['ptkid']);
	exit;
}

if($_REQUEST['carregadadositem']){
	header('content-type: text/html; charset=ISO-8859-1');
	$arItens = explode('_', $_POST['iteid']);
	
	if( $arItens[0] == 'P' ){
		
		$sql = "SELECT DISTINCT
				    d.dicvalor as valor, 
				    umdi.umidescricao as unidade
				FROM
				    emenda.itempar ip
				    inner join par.propostaitemcomposicao p ON ip.picid = p.picid
				    inner join par.detalheitemcomposicao d ON d.picid = p.picid
					inner join par.ufdetalheitemcomposicao ufd ON ufd.dicid = d.dicid
					inner join par.unidademedidadetalhamentoitem umdi on umdi.umiid = p.umiid and umdi.umistatus = 'A'
	                            inner join emenda.itempar_especificacao ite on ite.ipaid = ip.ipaid
				WHERE
				    ite.iteid = {$arItens[1]}
				    and d.dicstatus = 'A' 
				    and (cast(now() as date) between d.dicdatainicial and d.dicdatafinal) 
					and ufd.estuf in (SELECT DISTINCT enb.estuf
											FROM 
												emenda.planotrabalho ptr 
											    inner join emenda.entidadebeneficiada enb on enb.enbid = ptr.enbid
											WHERE 
												ptr.ptrid = {$_REQUEST['ptrid']})";
		$arrItem = $db->pegaLinha($sql);
		if( $arrItem ){ 
			$arrItem['unidade'] = iconv("iso-8859-1","utf-8",$arrItem['unidade']);		
			echo simec_json_encode( $arrItem );
		} else {
			echo '';
		}
	} else {
		echo '';
	}
	exit;
}

if($_REQUEST['cancelar']){
	header('content-type: text/html; charset=ISO-8859-1');
	$obPTA->verificaExistenciaKit($_POST['pteid']);
	exit;
}

$pteid = $_REQUEST['pteid'];
$iceid = $_REQUEST['iceid'];
$ptrid = $_REQUEST['ptrid'];

$obPTA->validaEspecificacaoPTA( $pteid, $ptrid );


if($_REQUEST['visualiza'] == "true"){
	$read = 'readonly="readonly"';
	$visualiza = "true";
}else{
	$read = '';	
	$visualiza = "";
}

$ptkquantidade = 0;

$arAcao = $obPTA->carregaDadosEspecificacaoKit( $pteid );

$ptridpai = $_SESSION["emenda"]["ptridpai"];
$refid = $_SESSION["emenda"]["refid"];
$ptrid = ($ptrid ? $ptrid : $_SESSION["emenda"]["ptrid"]);

$boGuia = $db->pegaUm("SELECT count(gptid) FROM emenda.guia_planotrabalho WHERE ptrid = {$ptrid}");
$boGuia = $boGuia == 0 ? 'false' : 'true';

?>
<html>
<head>
<style>

#loader-container,
#LOADER-CONTAINER{
    background: transparent;
    position: absolute;
    width: 100%;
    text-align: center;
    z-index: 8000;
    height: 100%;
}


#loader {
    background-color: #fff;
    color: #000033;
    width: 300px;
    border: 2px solid #cccccc;
    font-size: 12px;
    padding: 25px;
    font-weight: bold;
    margin: 150px auto;
}
</style>
</head>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<body marginwidth="0" marginheight="0" bgcolor="#ffffff" leftmargin="0" topmargin="0">

<div id="loader-container" style="display: none">
   	<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
</div>
	<form id="formulario" name="formulario" action="#" method="post" enctype="multipart/form-data" >
		
		<input type="hidden" name="hidTotal" id="hidTotal" value="<?=$arAcao['total']; ?>">
		<input type="hidden" name="hidVizualiza" id="hidVizualiza" value="<?=$visualiza; ?>">
		<input type="hidden" name="refid" id="refid" value="<?=$refid; ?>">
		<input type="hidden" name="ptridpai" id="ptridpai" value="<?=$ptridpai;?>">
		<input type="hidden" name="ptrid" id="ptrid" value="<?=$ptrid;?>">
		<input type="hidden" name="boGuia" id="boGuia" value="<?=$boGuia;?>">
		
		<input type="hidden" name="ptkid" id="ptkid" value="<?=$ptkid; ?>">
		
		<table id="tblform" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td class="SubTituloDireita" style="width:19.5%;">Ação:</td>
				<td><?=$arAcao['ininome'];?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita" style="width:19.5%;">Especificação:</td>
				<td><?=$arAcao['espnome'];?></td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Unidade de Medida:</td>
				<td><?=$arAcao['espunidademedida'];?></td>
			</tr>
		</table>
		<div id="lista"> <? $obPTA->popupCadastraKit($pteid, $read); ?></div>
	</form>
</body>
<div id="erro"></div>
<script type="text/javascript">
	if($('hidVizualiza').value == ""){
		retornaTotalGeral( $('hidTotal').value);
	}
		
	function retornaTotalGeral(valor){

		window.opener.$('ptevalorunitario').value = mascaraglobal('###.###.###.###,##', valor);
		window.opener.$('ptevalorunitarioH').value = mascaraglobal('###.###.###.###,##', valor);

		if(window.opener.$('ptequantidade').value == 0){
			window.opener.$('ptequantidade').value = 1;
		}
		window.opener.calculaTotal(valor);
		
		//if(window.opener.$('ptevalorproponente').value == ""){
			window.opener.$('concedente').innerHTML = 'R$ ' + mascaraglobal('###.###.###.###,##', valor);
			window.opener.$('hidConcedente').value = mascaraglobal('###.###.###.###,##', valor);
			window.opener.$('ptevalorproponente').value = 0;
		//}
		//window.opener.$('kitmodificado').value = 'true';
	}
	function pesquisar(){
		$('loader-container').show();
		$('itemdesc').innerHTML = '';
		$('itemdesc').style.display = 'none';
		$('item').style.display = '';
		var myajax = new Ajax.Request('emenda.php?modulo=principal/popupCadastrarKit&acao=A', {
			        method:     'post',
			        parameters: '&pesquisa=true&pteid='+$('pteid').value,
			        asynchronous: false,
			        onComplete: function (res){
						$('lista').innerHTML = res.responseText;
			        }
			  });
		$('loader-container').hide();
	}
	
	function calculaTotal(valor){
		var result = "";
		if( (valor) || ($('ptkvalorunitario').value != "") ){
			valor = $('ptkvalorunitario').value;
							
			valor = valor.replace(/\./gi,"");
			valor = valor.replace(/\,/gi,".");
			result = parseInt($('ptkquantidade').value) * valor;

			$('hidValorTotal').value = result;
			$('total').innerHTML = 'R$ ' + mascaraglobal('###.###.###.###,##', result.toFixed(2));
		}
	}
	
	function incluir(){		
		var nomeform 		= 'formulario';
		var submeterForm 	= false;
		var campos 			= new Array();
		var tiposDeCampos 	= new Array();
		
		if( $('btnIncluir').value != 'Alterar' ){
			if( $('tipoitem').value == 'item' ){
				campos[0] 			= "iteid";
			} else {
				campos[0] 			= "ptkdescricao";
			}
			campos[1] 			= "ptkunidademedida";
			campos[2] 			= "ptkquantidade";
			campos[3] 			= "ptkvalorunitario";
			
			if( $('tipoitem').value == 'item' ){		 
				tiposDeCampos[0] 	= "select";
			} else {
				tiposDeCampos[0] 	= "texto";
			}
			tiposDeCampos[1] 	= "texto";
			tiposDeCampos[2] 	= "texto";
			tiposDeCampos[3] 	= "texto";
		} else {
			campos[0] 			= "ptkunidademedida";
			campos[1] 			= "ptkquantidade";
			campos[2] 			= "ptkvalorunitario";
			
			tiposDeCampos[0] 	= "texto";
			tiposDeCampos[1] 	= "texto";
			tiposDeCampos[2] 	= "texto";
		}
		if(validaForm(nomeform, campos, tiposDeCampos, submeterForm ) ){
			
			var valor = $('ptkvalorunitario').value.replace(/\./gi,"");
			valor = valor.replace(/\,/gi,".");

			if(parseInt($('ptkquantidade').value) <= 0){
				alert('A quantidade não pode se menor ou igual a 0!');
				$('ptkquantidade').focus();
				return false;
			}else if( valor <= 0){
				alert('O valor unitário não pode se menor ou igual a 0!');
				$('ptkvalorunitario').focus();
				return false;
			}
			$('loader-container').show();
			var req = new Ajax.Request('emenda.php?modulo=principal/popupCadastrarKit&acao=A', {
			        method:     'post',
			        parameters: '&inserekit=true&'+$('formulario').serialize(),
			        asynchronous: false,
			        onComplete: function (res){
			        	//$('erro').innerHTML = res.responseText;		        	
						if(res.responseText == "1" ){
							$('ptkid').value = "";
							alert('Operação realizada com sucesso!');
							pesquisar(); 
							retornaTotalGeral($('hidValorTotalGeral').value);
						}else if(res.responseText == "2"){
							alert('Não pode haver a mesma descrição para o mesmo kit');
						}else{
							if(res.responseText == 'filho'){
								alert('Operação realizada com sucesso! \nPor perda de informações, você será redirecionado para a tela de lista de iniciativa do PTA');
								window.opener.location.href = "emenda.php?modulo=principal/acoesPlanoTrabalho&acao=A"; 
							} else {
 								alert('Operação não realizada!');
 							}
 						}
			        }
			  });
			$('loader-container').hide();
		}
		
	}
	function alterar(ptkid, celula){
		var retorno = "";
		
		var tabela = $('tb_listaKit');
		
		for(i=2; i<tabela.rows.length; i++){
			if(i % 2){
				tabela.rows[i].style.backgroundColor = "#f7f7f7";				
			} else {
				tabela.rows[i].style.backgroundColor = "";
			}
		}
		celula.parentNode.parentNode.parentNode.style.background = '#ffffcc';
		
		$('loader-container').show();
		var req = new Ajax.Request('emenda.php?modulo=principal/popupCadastrarKit&acao=A', {
		        method:     'post',
		        parameters: '&alterar=true&ptkid='+ptkid+'&ptrid='+$('ptrid').value,
		        asynchronous: false,
		        onComplete: function (res){
					//$('erro').innerHTML = res.responseText;
					var json = res.responseText.evalJSON();
					
					$('btnIncluir').value = 'Alterar';

					$('ptkid').value = json.ptkid;
					$('pteid').value = json.pteid;
					//$('iteid').value = json.iteid;
					
					if( $('tipoitem').value == 'item' ){
						$('itemdesc').innerHTML = '<input type="hidden" name="iteid" id="iteid" value="'+json.iteid+'">'+json.ptkdescricao;
						$('itemdesc').style.display = '';
						$('item').style.display = 'none';
					} else {
						$('ptkdescricao').value = json.ptkdescricao;
						$('itemdesc').style.display = 'none';
						$('item').style.display = '';
					}
					
					$('ptkunidademedida').value = json.ptkunidademedida;
					$('ptkquantidade').value = json.ptkquantidade;
					$('ptkvalorunitario').value = mascaraglobal('###.###.###.###,##', json.ptkvalorunitario ) ;
					$('total').update('R$ ' + mascaraglobal('###.###.###.###,##', json.total));
					if( $('boGuia').value == 'true' ){
						$('ptkvalorunitario').disabled = true;
					} else {
						$('ptkvalorunitario').disabled = false;
					}
		        }
		  });
		$('loader-container').hide();
	}
	
	function carregaDadosItem(iteid){
		$('loader-container').show();
		var req = new Ajax.Request('emenda.php?modulo=principal/popupCadastrarKit&acao=A', {
		        method:     'post',
		        parameters: '&carregadadositem=true&iteid='+iteid+'&ptrid='+$('ptrid').value,
		        asynchronous: false,
		        onComplete: function (res){
					
					//$('erro').innerHTML = res.responseText;
					if( res.responseText != '' ){
						var json = res.responseText.evalJSON();
						
						$('ptkunidademedida').value = json.unidade;
						$('ptkvalorunitario').value = mascaraglobal('###.###.###.###,##', json.valor) ;
						if( json.unidade != '' ){
							$('ptkunidademedida').disabled = true;
							$('unidade').innerHTML = '<input type="hidden" name="ptkunidademedida" id="ptkunidademedida" value="'+json.unidade+'">';
						} else {
							$('ptkunidademedida').disabled = false;
							$('unidade').innerHTML = '';
						}
						if( json.valor != '' ){
							$('ptkvalorunitario').disabled = true;
							$('valor').innerHTML = '<input type="hidden" name="ptkvalorunitario" id="ptkvalorunitario" value="'+mascaraglobal('###.###.###.###,##', json.valor)+'">';
						} else {
							$('ptkvalorunitario').disabled = false;
							$('valor').innerHTML = '';
						}
					} else {
						$('ptkunidademedida').value = '';
						$('ptkvalorunitario').value = '';
						$('ptkunidademedida').disabled = false;
						$('ptkvalorunitario').disabled = false;
					}
		        }
		  });
		$('loader-container').hide();
	}

	function excluir(ptkid){
		if(confirm('Deseja realmente excluir este item?')) {
			$('loader-container').show();
			var req = new Ajax.Request('emenda.php?modulo=principal/popupCadastrarKit&acao=A', {
			        method:     'post',
			        parameters: '&excluir=true&ptkid='+ptkid+'&pteid=<?=$pteid ?>',
			        asynchronous: false,
			        onComplete: function (res){		        	
						if(res.responseText == "1" ){
							alert('Operação realizada com sucesso!');
							pesquisar();
							retornaTotalGeral($('hidValorTotalGeral').value);
							limpaCampos();
						}else{
							alert('Operação não realizada!');
						}
						//$('erro').innerHTML = res.responseText;
			        }
			  });
			$('loader-container').hide();
		}
	}
	function Cancelar(){
        pesquisar();
	}
	function fecharKit(){
		if( $('hidVizualiza').value != 'true' ){
			alert("Para salvar os dados inseridos no kit, você deve concluir o preenchimento da especificação e clicar no botão \"Salvar\"");
		}
		window.close();
	}
</script>
</html>