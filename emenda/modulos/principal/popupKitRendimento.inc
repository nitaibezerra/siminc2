<?php
include_once( APPRAIZ . 'emenda/classes/PTA.class.inc');
$obPTA = new PTA();

//$obPTA->validaSessionPTA( $_REQUEST['ptrid'] );

if($_REQUEST['pesquisa']){
	header('content-type: text/html; charset=ISO-8859-1');
	popupCadastraKit($_POST['perid']);
	exit;
}

if($_REQUEST['inserekit']){
	header('content-type: text/html; charset=ISO-8859-1');
	insereKitPlanoTrabalho($_POST);
	exit;
}

if($_REQUEST['excluir']){
	header('content-type: text/html; charset=ISO-8859-1');
	
	$sql = "DELETE FROM emenda.ptitemkit WHERE ptkid = {$_POST['ptkid']}";				  
	$db->executar($sql);
	
	if( $db->commit() ){
		$sql1 = "(SELECT COALESCE(sum(ptkquantidade * ptkvalorunitario),0) as valorUnitario FROM emenda.ptitemkit WHERE perid = {$_POST['perid']})";

		$sql = "UPDATE emenda.ptaespecificacaorendimento SET
					  pervalorunitario = ".$sql1."						 
					WHERE perid = {$_POST['perid']}";
		
		$db->executar($sql);
		$ret = $db->commit();
	}
	echo $ret;
	exit;
}

if($_REQUEST['alterar']){
	
	header('content-type: text/html; charset=ISO-8859-1');
	if( $_SESSION['exercicio'] >= '2012' ){
		$sql = "SELECT ptkid, perid, iteid, ptkdescricao, ptkunidademedida, ptkquantidade, ptkvalorunitario, total FROM (
					SELECT
						ptk.ptkid,
						ptk.perid,
						'P_'||ptk.iteid as iteid,
		                case when pic.picdescricao is not null then pic.picdescricao else ptk.ptkdescricao end as ptkdescricao,
						ptk.ptkunidademedida,
						ptk.ptkquantidade,
						ptk.ptkvalorunitario,
						trim(to_char( (ptk.ptkquantidade * ptk.ptkvalorunitario), '999G999G999D99')) as total
					FROM 
					  emenda.ptitemkit ptk
	                  left join emenda.itempar_especificacao ipe on ipe.iteid = ptk.iteid
	                  left join emenda.itempar itp on itp.ipaid = ipe.ipaid
	                  left join par.propostaitemcomposicao pic on pic.picid = itp.picid
				UNION
					SELECT
						  ptk.ptkid,
						  ptk.perid,
		                  'O_'||ptk.itoid as iteid,
		                  case when pto.ptodescricao is not null then pto.ptodescricao else ptk.ptkdescricao end as ptkdescricao,
						  ptk.ptkunidademedida,
						  ptk.ptkquantidade,
						  ptk.ptkvalorunitario,
						  trim(to_char( (ptk.ptkquantidade * ptk.ptkvalorunitario), '999G999G999D99')) as total
						FROM 
						  emenda.ptitemkit ptk
		                  left join emenda.itempar_especificacao ipe on ipe.itoid = ptk.itoid
		                  left join emenda.itemobras io on io.itoid = ipe.itoid
		                  left join obras.pretipoobra pto on pto.ptoid = io.ptoid
				) as foo
				WHERE 
					ptkid = {$_POST['ptkid']}
					and ptkdescricao is not null
			ORDER BY ptkdescricao";
	} else {
		$sql = "SELECT
				  ptk.ptkid,
				  ptk.perid,
				  ptk.iteid,
                  ptk.itoid,
                  ptk.ptkdescricao,
				  ptk.ptkunidademedida,
				  ptk.ptkquantidade,
				  ptk.ptkvalorunitario,
				  trim(to_char( (ptk.ptkquantidade * ptk.ptkvalorunitario), '999G999G999D99')) as total
				FROM 
				  emenda.ptitemkit ptk
				WHERE 
					ptkid = {$_POST['ptkid']}";
	}
		
	$arDados = $db->pegaLinha( $sql );
	$arDados['ptkdescricao'] = iconv("iso-8859-1","utf-8",$arDados['ptkdescricao']);
	$arDados['ptkunidademedida'] = iconv("iso-8859-1","utf-8",$arDados['ptkunidademedida']);
	
	echo simec_json_encode( $arDados );
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
	$obPTA->verificaExistenciaKit($_POST['perid']);
	exit;
}

$perid = $_REQUEST['perid'];
$iceid = $_REQUEST['iceid'];
$ptrid = $_REQUEST['ptrid'];

validaEspecificacaoPTA( $perid, $ptrid );


if($_REQUEST['visualiza'] == "true"){
	$read = 'readonly="readonly"';
	$visualiza = "true";
}else{
	$read = '';	
	$visualiza = "";
}

$ptkquantidade = 0;

$sql = "SELECT e.espnome, 
		    e.espunidademedida,
		    i.ininome,
            sum(ptk.ptkquantidade * ptk.ptkvalorunitario) as total
		FROM emenda.especificacao e 
			INNER JOIN emenda.iniciativaespecificacao ie ON (e.espid = ie.espid) 
			INNER JOIN emenda.iniciativa i ON (ie.iniid = i.iniid) 
			INNER JOIN emenda.ptaespecificacaorendimento ptie ON (ie.iceid = ptie.iceid) 
			LEFT JOIN emenda.ptitemkit ptk ON ptk.perid = ptie.perid
			inner join emenda.especificacao_programacaoexercicio epe on epe.espid = e.espid and epe.prsano = '".$_SESSION['exercicio']."'
		WHERE ptie.perid = $perid
        GROUP BY
        	e.espnome, 
		    e.espunidademedida,
		    i.ininome";		    
$arAcao = $db->pegaLinha($sql);


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
		<div id="lista"> <? popupCadastraKit($perid, $read); ?></div>
	</form>
</body>
<div id="erro"></div>
<script type="text/javascript">
	if($('hidVizualiza').value == ""){
		retornaTotalGeral( $('hidTotal').value);
	}
		
	function retornaTotalGeral(valor){
		
		window.opener.document.getElementById('pervalorunitario').value = mascaraglobal('###.###.###.###,##', valor);

		if(window.opener.document.getElementById('perquantidade').value == 0){
			window.opener.document.getElementById('perquantidade').value = 1;
		}
		
		window.opener.calculaTotal(valor);
		
		//if(window.opener.$('pervalorrendimento').value == ""){
			//window.opener.document.getElementById('concedente').innerHTML = 'R$ ' + mascaraglobal('###.###.###.###,##', valor);
			//window.opener.document.getElementById('hidConcedente').value = mascaraglobal('###.###.###.###,##', valor);
			//window.opener.document.getElementById('pervalorrendimento').value = 0;
		//}
		//window.opener.$('kitmodificado').value = 'true';
	}
	function pesquisar(){
		$('loader-container').show();
		$('itemdesc').innerHTML = '';
		$('itemdesc').style.display = 'none';
		$('item').style.display = '';
		var myajax = new Ajax.Request('emenda.php?modulo=principal/popupKitRendimento&acao=A', {
			        method:     'post',
			        parameters: '&pesquisa=true&perid='+$('perid').value,
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
			if(!window.opener.verificaValorTotal( $('hidTotal').value )){
				return false;
			}
			
			$('loader-container').show();
			var req = new Ajax.Request('emenda.php?modulo=principal/popupKitRendimento&acao=A', {
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
		var req = new Ajax.Request('emenda.php?modulo=principal/popupKitRendimento&acao=A', {
		        method:     'post',
		        parameters: '&alterar=true&ptkid='+ptkid+'&ptrid='+$('ptrid').value,
		        asynchronous: false,
		        onComplete: function (res){
					//$('erro').innerHTML = res.responseText;
					var json = res.responseText.evalJSON();
					
					$('btnIncluir').value = 'Alterar';

					$('ptkid').value = json.ptkid;
					$('perid').value = json.perid;
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
		var req = new Ajax.Request('emenda.php?modulo=principal/popupKitRendimento&acao=A', {
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
			var req = new Ajax.Request('emenda.php?modulo=principal/popupKitRendimento&acao=A', {
			        method:     'post',
			        parameters: '&excluir=true&ptkid='+ptkid+'&perid=<?=$perid ?>',
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
<?
function popupCadastraKit($perid, $read = null){
	global $db;
	
	monta_titulo('Detalhamento dos ítens', '');		
	
	$html = '';
	if($read){
		$html .= '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
				<thead>
					<tr>				
						<td rowspan="2" align="Center" class="title"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Identificação dos Itens</strong></td>
							
						<td rowspan="2" align="Center" class="title"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Unidade de Medida</strong></td>
							
						<td rowspan="2" align="Center" class="title"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Quantidade</strong></td>
							
						<td colspan="2"  align="Center" class="title"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Estimativa de Custo</strong></td>
					</tr>
					<tr>
						<td align="Center" class="title"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Valor Unitário</strong></td>
						<td align="Center" class="title"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Valor Total</strong></td>
					</tr>
					</thead>';

			
					
			$arDados = carregaPtitemkitDAO( $perid );
			
			if($arDados){
				foreach ($arDados as $key => $value) {
					$key % 2 ? $cor = "" : $cor = "#f7f7f7";
					
					$html.= '<tr bgcolor="'.$cor.'" onmouseout="this.bgColor=\''.$cor.'\'" onmouseover="this.bgColor=\'#ffffcc\';">
								<td>'.$value['ptkdescricao'].'</td>
								<td>'.$value['ptkunidademedida'].'</td>
								<td style="text-align: center;">'.$value['ptkquantidade'].'</td>
								<td style="text-align: center;">R$ '.number_format($value['ptkvalorunitario'],2,',','.').'</td>
								<td style="text-align: center;">R$ '.number_format($value['total'],2,',','.').'</td>
							</tr>';
		
					// Soma os valores para o totalizador
					$totalQuantidade += $value['ptkquantidade'];
					$totalValorUnit += $value['ptkvalorunitario'];
					$totalizadorItens += $value['total'];
				}
				
				$html.= '<thead>
							<tr>
								<td colspan="2" style="text-align: right;"><b>Totalizador:</b></td>
								<td style="text-align: center;color: rgb(0, 102, 204);"><b>'.$totalQuantidade.'</b></td>
								<td style="text-align: center;color: rgb(0, 102, 204);"><b>R$ '.number_format($totalValorUnit,2,',','.').'</b></td>
								<td style="text-align: center;color: rgb(0, 102, 204);"><b>R$ '.number_format($totalizadorItens,2,',','.').'</b></td>
							</tr>
							</thead>
						</table>
						<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
							<tr bgcolor="#ffffff">
								<td><b>Total de Registros: '.count($arDados).'</b></td>
								<td></td>
							</tr>
						</table>';
		}
	}else{
		$html.= '<input type="hidden" value="'.$perid.'" name="perid" id="perid">
				<input type="hidden" name="hidValorTotal" id="hidValorTotal" value="">
				
				<table width="95%" id="tb_listaKit" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
				<thead>
					<tr>				
						<td rowspan="2" align="Center" class="title"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Identificação dos Itens</strong></td>
							
						<td rowspan="2" align="Center" class="title"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Unidade de Medida</strong></td>
							
						<td rowspan="2" align="Center" class="title"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Quantidade</strong></td>
							
						<td colspan="2"  align="Center" class="title"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Estimativa de Custo</strong></td>
							
						<td rowspan="2" align="Center" class="title"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Ação</strong></td>
					</tr>
					<tr>
						<td align="Center" class="title"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Valor Unitário</strong></td>
						<td align="Center" class="title"
							style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
							onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';"><strong>Valor Total</strong></td>
					</tr>
					</thead>';
		//campo_texto( 'ptkdescricao', 'S', 'S', 'Identificação dos Itens', 50, 500, '', '','','','','id="ptkdescricao"','','','')
		if( $_SESSION['exercicio'] >= '2012' ){
		$sqlItem = "select  tipo||'_'||codigo as codigo, descricao from (
						SELECT 
													ipe.iteid as codigo,
												    pic.picdescricao as descricao,
						                            'P' as tipo
												FROM emenda.itempar gip
													inner join par.propostaitemcomposicao pic on pic.picid = gip.picid
						                            inner join emenda.itempar_especificacao ipe on ipe.ipaid = gip.ipaid
												WHERE
													pic.picstatus = 'A'
						                            and ipe.espid in (select espid from emenda.iniciativaespecificacao ie
																		inner join emenda.ptaespecificacaorendimento pe on pe.iceid = ie.iceid where pe.perid = $perid and ie.icestatus = 'A')
													and ipe.iteid not in (select iteid from emenda.ptitemkit p where p.perid = $perid and iteid is not null)
												UNION
						                        SELECT
						                            io.itoid as codigo,
						                            pto.ptodescricao as descricao,
						                            'O' as tipo
						                        FROM
						                            emenda.itemobras io
						                            inner join obras.pretipoobra pto on pto.ptoid = io.ptoid
						                            inner join emenda.itempar_especificacao ipe on ipe.itoid = io.itoid
						                        WHERE
						                            pto.ptostatus = 'A'
						                            and ipe.espid in (select espid from emenda.iniciativaespecificacao ie
																		inner join emenda.ptaespecificacaorendimento pe on pe.iceid = ie.iceid where pe.perid = $perid and ie.icestatus = 'A')
													and io.itoid not in (select itoid from emenda.ptitemkit p where p.perid = $perid and itoid is not null)
						) as foo
						order by
							descricao";
		$arrItem = $db->carregar($sqlItem);
		} else {
			$arrItem = array();
		}
		
		$html.= '<tr>
			<td><div id="item" style="display: \'\'">'.($arrItem ? $db->monta_combo("iteid",$sqlItem, 'S','-- Selecione --','carregaDadosItem', '', '',250,'S','iteid', true, '', 'Identificação dos Itens') . '<input type="hidden" value="item" name="tipoitem" id="tipoitem">' : campo_texto( 'ptkdescricao', 'S', 'S', 'Identificação dos Itens', 50, 500, '', '','','','','id="ptkdescricao"','','','') . '<input type="hidden" value="desc" name="tipoitem" id="tipoitem">' ).'</div><div id="itemdesc" style="display: none"></div></td>
			<td>'.campo_texto( 'ptkunidademedida', 'S', 'S', 'Unidade de Medida', 30, 100, '', '','','','','id="ptkunidademedida"','','','').'<div id="unidade"></div></td>
			<td><input id="ptkquantidade" class="normal" type="text" title="Quantidade" style="width: 23ex;" onblur="MouseBlur(this); this.value=mascaraglobal(\'[#]\',this.value); calculaTotal(\'\');" 
					onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" 
					onkeyup="this.value=mascaraglobal(\'[#]\',this.value);" value="0" maxlength="15" size="21" name="ptkquantidade"/>
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>
			<td>'.campo_texto('ptkvalorunitario', 'S', 'S', 'Valor Unitário', 20, 13, '[###.]###,##', '','','','','id="ptkvalorunitario"', '', '', "this.value=mascaraglobal('[###.]###,##',this.value); calculaTotal('');").'<div id="valor"></div></td>
			<td style="text-align: center;"><div id="total">R$ 0,00</div></td>
			<td style="text-align: center;"><input type="button" value="Incluir" name="btnIncluir" id="btnIncluir" onclick="incluir();">
											<input type="button" value="Cancelar" name="btnCancelar" id="btnCancelar" onclick="Cancelar();"></td>
		</tr>';
		
		$acao = "( '<center><img src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"alterar('|| ptkid || ', this)\" \" border=0 alt=\"Ir\" title=\"Alterar\">'  || 
				            '<img src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"excluir('|| ptkid ||');\" border=0 alt=\"Ir\" title=\"Excluir\"></center>' )";
						
		$arDados = carregaPtitemkitDAO( $perid, $acao );
		$valorTotal = 0;
		if($arDados){
			foreach ($arDados as $key => $value) {
				$key % 2 ? $cor = "" : $cor = "#f7f7f7";
				$html.= '<tr bgcolor="'.$cor.'" onmouseout="this.bgColor=\''.$cor.'\'" onmouseover="this.bgColor=\'#ffffcc\'">
							<td>'.$value['ptkdescricao'].'</td>
							<td>'.$value['ptkunidademedida'].'</td>
							<td style="text-align: center;">'.$value['ptkquantidade'].'</td>
							<td style="text-align: center;">R$ '.number_format($value['ptkvalorunitario'],2,',','.').'</td>
							<td style="text-align: center;">R$ '.number_format($value['total'],2,',','.').'</td>
							<td style="text-align: center;">'.$value['acao'].'</td>
						</tr>';
				
				// Soma os valores para o totalizador
				$totalQuantidade += $value['ptkquantidade'];
				$totalValorUnit += $value['ptkvalorunitario'];
				$totalizadorItens += $value['total'];
				$valorTotal+=$value['total'];
			}
			
			$html.= '<tr>
						<td colspan="2" style="text-align: right; background: #E3E3E3"><b>Totalizador:</b></td>
						<td style="text-align: center;color: rgb(0, 102, 204); background: #E3E3E3"><b>'.$totalQuantidade.'</b></td>
						<td style="text-align: center;color: rgb(0, 102, 204); background: #E3E3E3"><b>R$ '.number_format($totalValorUnit,2,',','.').'</b></td>
						<td style="text-align: center;color: rgb(0, 102, 204); background: #E3E3E3"><b>R$ '.number_format($totalizadorItens,2,',','.').'</b></td>
					</tr>
		
				</table>
				<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">
					<tr bgcolor="#ffffff">
						<td><b>Total de Registros: '.count($arDados).'</b></td>
						<td></td>
					</tr>
				</table>';
		}
		echo '<input type="hidden" name="hidValorTotalGeral" id="hidValorTotalGeral" value="'.number_format($valorTotal,2, ',', '.').'">';
	}
	$html.= '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" >
				<tr>
					<td>&nbsp;</td>
				</tr>
				<tr bgcolor="#ffffff">
					<td style="text-align: center;"><input type="button" name="fechar" value="Fechar" onclick="javascript: fecharKit();"></td>
				</tr>
			</table>';
	
	echo $html;
}

function carregaPtitemkitDAO($perid, $acao = null){
	global $db;
	$acao = $acao ? "$acao as acao," : ''; 
	if( $_SESSION['exercicio'] >= '2012' ){
	$sql = "SELECT ".($acao ? "acao," : '')." ptkid, perid, iteid, itoid, ptkdescricao, ptkunidademedida, ptkquantidade, ptkvalorunitario, total FROM (
			SELECT
				$acao
							  ptk.ptkid,
							  ptk.perid,
							  ptk.iteid,
			                  ptk.itoid,
			                  case when pic.picdescricao is not null then pic.picdescricao else ptk.ptkdescricao end as ptkdescricao,
							  ptk.ptkunidademedida,
							  ptk.ptkquantidade,
							  ptk.ptkvalorunitario,
							  (ptk.ptkquantidade * ptk.ptkvalorunitario) as total
							FROM 
							  emenda.ptitemkit ptk
			                  left join emenda.itempar_especificacao ipe on ipe.iteid = ptk.iteid
			                  left join emenda.itempar itp on itp.ipaid = ipe.ipaid
			                  left join par.propostaitemcomposicao pic on pic.picid = itp.picid
			UNION
				SELECT
				$acao
							  ptk.ptkid,
							  ptk.perid,
							  ptk.iteid,
			                  ptk.itoid,
			                  case when pto.ptodescricao is not null then pto.ptodescricao else ptk.ptkdescricao end as ptkdescricao,
							  ptk.ptkunidademedida,
							  ptk.ptkquantidade,
							  ptk.ptkvalorunitario,
							  (ptk.ptkquantidade * ptk.ptkvalorunitario) as total
							FROM 
							  emenda.ptitemkit ptk
			                  left join emenda.itempar_especificacao ipe on ipe.itoid = ptk.itoid
			                  left join emenda.itemobras io on io.itoid = ipe.itoid
			                  left join obras.pretipoobra pto on pto.ptoid = io.ptoid
			) as foo
			WHERE 
				perid = $perid
				and ptkdescricao is not null
			ORDER BY ptkdescricao";
	} else {
		$sql = "SELECT
				$acao
				  ptk.ptkid,
				  ptk.perid,
				  ptk.iteid,
                  ptk.itoid,
                  ptk.ptkdescricao,
				  ptk.ptkunidademedida,
				  ptk.ptkquantidade,
				  ptk.ptkvalorunitario,
				  (ptk.ptkquantidade * ptk.ptkvalorunitario) as total
				FROM 
				  emenda.ptitemkit ptk
				WHERE 
					ptk.perid = $perid";
	}
	return $db->carregar($sql);
}

function validaEspecificacaoPTA( $perid, $ptrid ){
	global $db;
	if( !$perid ){
		echo "<script>
				alert('Número da especificação e inválido.');
				window.close();
			  </script>";
		die;
	} else {
		if( is_numeric( $perid ) ){
			$sql = "SELECT 
					count(ptie.perid) as total
				FROM 
					emenda.ptiniciativa pti
					inner join emenda.ptaespecificacaorendimento ptie
				  		on ptie.ptiid = pti.ptiid
				WHERE
					pti.ptrid = {$ptrid}
				    AND ptie.perid = $perid";
			
			$boExiste = $db->pegaUm($sql);
		} else {
			$boExiste = 0;
		}
		
		if($boExiste == 0){
			echo "<script>
					alert('Este número da especificação e inválido para o plano de trabalho selecionado.');
					window.close();
				  </script>";
			die();
		} else {
			return true;
		}
	}
}

function insereKitPlanoTrabalho($post){
	global $db;
	
	$valorUnit = str_replace('.','', $post['ptkvalorunitario']);
	$valorUnit = str_replace(',','.', $valorUnit);
	
	if( isset($post['iteid']) ){
		$arItens = explode('_', $post['iteid']);
		if( $arItens[0] == 'P' ){
			$item = $arItens[1];
			$obras = 'null'; 
		} else {
			$item = 'null';
			$obras = $arItens[1];
		}
		$ptkdescricao = 'null';
	} else {
		$obras = 'null';
		$item = 'null';
		$ptkdescricao = "'".utf8_decode($post['ptkdescricao'])."'";
	}
	
	if($post['ptkid'] ){
		
		$sql = "UPDATE 
				  emenda.ptitemkit  
				SET 
				  perid = {$post['perid']},
				  iteid = $item,
				  itoid = $obras,
				  ptkdescricao = $ptkdescricao,
				  ptkunidademedida = '".pg_escape_string(utf8_decode($post['ptkunidademedida']))."',
				  ptkquantidade = {$post['ptkquantidade']}
				  $filtro				 
				WHERE 
				  ptkid = {$post['ptkid']}";	
		
		$db->executar( $sql );
		$ret = $db->commit();
		
		if($ret == true){
			$sql1 = "(SELECT COALESCE(sum(ptkquantidade * ptkvalorunitario),0) as valorUnitario FROM emenda.ptitemkit WHERE perid = {$post['perid']})";
	
			$sql = "UPDATE emenda.ptaespecificacaorendimento SET
						  pervalorunitario = ".$sql1."						 
						WHERE perid = {$post['perid']}";
			
			$db->executar($sql);
			$db->commit();
		}
		echo trim($ret);		
	}else{
		
		$total = 0; 
		if($total == 0){			
			$sql = "INSERT INTO 
					  emenda.ptitemkit(
					  perid,
					  iteid,
					  itoid,
					  ptkdescricao,
					  ptkunidademedida,
					  ptkquantidade,
					  ptkvalorunitario
					) 
					VALUES (
					  {$post['perid']},
					  $item,
					  $obras,
					  $ptkdescricao,
					  '".pg_escape_string(utf8_decode($post['ptkunidademedida']))."',
					  {$post['ptkquantidade']},
					  $valorUnit
					) RETURNING ptkid";
		
			$ret = $db->pegaUm( $sql );
			
			if( is_numeric( $ret )){
				$sql1 = "(SELECT COALESCE(sum(ptkquantidade * ptkvalorunitario),0) as valorUnitario FROM emenda.ptitemkit WHERE perid = {$post['perid']})";
	
				$sql = "UPDATE emenda.ptaespecificacaorendimento SET
							  pervalorunitario = ".$sql1."						 
							WHERE perid = {$post['perid']}";
				
				$db->executar($sql);
				$db->commit();
				
				echo 1;
			} else {						
				echo trim($ret);
			}
		}else{
			echo 2;
		}
	}
}

?>