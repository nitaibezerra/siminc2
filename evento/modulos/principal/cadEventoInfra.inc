<?php

$data_nova_regra_evento = strtotime(DATA_NOVO_REGRA_EVENTO_NOVA_INFRA);
$data_inclusao_evento = strtotime($_SESSION['evento']['evedatainclusao']);

if( $data_inclusao_evento >= $data_nova_regra_evento ){
    header( "location: evento.php?modulo=principal/eventos/cad_eve_infra&acao=A" );
}

error_reporting(0);
ini_set('display_errors', FALSE); 

if(!$_SESSION['evento']['eveid']){
    echo '
        <script>
            location.href="evento.php?modulo=principal/inicioEvento&acao=A";
        </script>
    ';	
    die();
}
 
$docid = evtCriarDoc();
 
if( $_POST['valorAjax'] ){
	$tipo  = $_GET['salvaTipo']; 
	$tieid = $_GET['tieid'];
	$valor = $_POST['valorAjax'];
	
	$sql = "
			UPDATE evento.tipoitemevento
			SET
				$tipo = $valor
			WHERE
				tieid = $tieid			
	"; 
    $ins = $db->executar( $sql );
    $db->commit($sql);
    echo("$valor");
    die();
}

if( $_REQUEST['excluir'] != '' ){
	$sql = "UPDATE evento.itemevento SET ievstatus = 'I' WHERE ievid = '{$_REQUEST['excluir']}'";
	$up  = $db->executar( $sql );
	$db->commit(); 
	redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'] );
}

if( $_REQUEST['montaComboUnidade'] == 'S' ){
	$sql ="
			SELECT 
				umeid as codigo,
				umedescricao as descricao
			FROM 
				evento.unidademedida
			WHERE
				umestatus = 'A'	
	";
	die($db->monta_combo("umeid",$sql,'S',"Selecione a Unidade de Medida",'','', '', 260, 'S', 'umeid'));
}

if( $_POST['ajaxitem'] ){
	if( $_GET['tipo'] == 'fixo'){
		$sql = "SELECT
	                			it.tieid, 
	                			it.tieitem,
	                			it.tiedescricao,
	                			it.tiedetalhe,
	                			it.umeid,
	                			it.tievalordf,
	                			it.tievaloroutrauf,
	                			it.tieidpai,
	                			it.tieitemtemvalor,
	                			it.tieitemtemcotacao,
	                			it.tiastatus,
	                			un.umedescricao
	                		FROM 	
	                			evento.tipoitemevento AS it
	                		LEFT JOIN
	                			evento.unidademedida AS un ON it.umeid = un.umeid
	                		WHERE tieid = '{$_POST['ajaxitem']}' 
	                		 ";
		
		$rsAjax = $db->carregar( $sql );
		
		$resp = $rsAjax[0]['tieid'].'_'.$rsAjax[0]['umedescricao'].'_'.$rsAjax[0]['tievalordf'].'_'.$rsAjax[0]['tievaloroutrauf'].'_'.$rsAjax[0]['tiedescricao'].'_'.$rsAjax[0]['tieitemtemcotacao'];
		echo $resp;
		die();
                
	} elseif($_GET['tipo'] == 'cadastrado') {
		
		$sql = "SELECT
                	it.tieid, 
                	it.tieitem,
                	it.tiedescricao,
                	it.tiedetalhe,
                	it.umeid,
                	it.tievalordf,
                	it.tievaloroutrauf,
                	it.tieidpai,
                	it.tieitemtemvalor,
                	it.tieitemtemcotacao,
                	it.tiastatus,
                	un.umedescricao,
                	i.ievvalordf,
                	i.ievid,
                	i.ievparceria
                FROM 	
                	evento.tipoitemevento AS it
                LEFT JOIN
                	evento.unidademedida AS un ON it.umeid = un.umeid
                INNER JOIN
                	evento.itemevento AS i ON i.tieid = it.tieid
                	
                WHERE it.tieid  = '{$_POST['ajaxitem']}' AND i.eveid = '{$_SESSION['evento']['eveid']}' AND i.ievstatus = 'A'    		 
                "; 
	   	$rsAjax = $db->carregar( $sql ); 

	   	$sql2 = "
	   			SELECT 
	   				di.dieid,
	   				di.diedia,
	   				di.diequantidade,
	   				di.dievalorunitario
	   			FROM 
	   				evento.itemevento as ie
                LEFT JOIN evento.detalheitemevento AS di
                	ON di.ievid = ie.ievid
                WHERE ie.tieid  = '{$_POST['ajaxitem']}'
                AND
                	di.diedia IS NOT NULL
                AND ie.ievstatus = 'A'
				AND ie.eveid = {$_SESSION['evento']['eveid']}
                ORDER BY di.diedia
	   	";   	
        $d = $db->carregar( $sql2 );
 		if( $d ){
 			$arr = array();
 			for( $i = 0; $i<count($d); $i++){
 				
 				if(  $d[$i]['diequantidade'] != ''){ 
 					$arr[] =  $d[$i]['diequantidade'];
 					$dias = implode(",", $arr);
 				}
 			}
 		}   
//                /ver( $rsAjax, $d, $dias, d );
//1124_Cotar valor unitário_4.75_4.75_Água Mineral em garrafas e/ou copos individuais Ver item 4.6.1 do TR_t_f_0,6_62422_f
$resp = $rsAjax[0]['tieid'].'_'.$rsAjax[0]['umedescricao'].'_'.$rsAjax[0]['tievalordf'].'_'.$rsAjax[0]['tievaloroutrauf'].'_'.$rsAjax[0]['tiedescricao'].'_'.$rsAjax[0]['tieitemtemcotacao'].'_'.$rsAjax[0]['ievvalordf'].'_'.$dias.'_'.$rsAjax[0]['ievid'].'_'.$rsAjax[0]['ievparceria'];
  		echo $resp;
		die();
	}
}

if( $_POST['action'] == 'grava' ){
	salva();
}

function salva(){
	global $db; 
	
	if($_REQUEST['update'] && is_numeric($_REQUEST['update'])){
		
		$sqlDel2 = "DELETE FROM evento.detalheitemevento WHERE ievid = '{$_REQUEST['update']}'";		
		$del 	 = $db->executar($sqlDel2);
		$sqlDel  = "DELETE FROM evento.itemevento WHERE tieid = '{$_POST['tipoitemevento'][0]}' AND eveid = '{$_SESSION['evento']['eveid']}' RETURNING ievid";
 		$ievid 	 = $db->pegaUm($sqlDel);
	} 
	if( !$_REQUEST['umeid'] ){ 
		$sqlexiste = "SELECT ievid FROM evento.itemevento WHERE eveid = '{$_SESSION['evento']['eveid']}'AND tieid = '{$_POST['tipoitemevento'][0]}' and ievstatus = 'A'";
		$existe = $db->pegaUm($sqlexiste);
	}
	if(!$existe){
		
		$_REQUEST['ievparceria'] = $_REQUEST['ievparceria'] == true ? $_REQUEST['ievparceria'] : 'false';
		
		if( $_REQUEST['umeid'] != 0){
			$value = $_POST['valor_cotado_add'];
			$sql = "
				INSERT INTO evento.itemevento
					(
						eveid,
						ievdescricao,
						ievvalordf,
						ievdatainclusao,
						ievstatus, 
						umeid,
						ievparceria
					)
				VALUES
					(
						'{$_SESSION['evento']['eveid']}',
						'{$_POST['hid_itemAdd']}',
						'{$_POST['tipoValor']}',
						'now()', 
						'A', 
						'{$_POST['umeid']}',
						'{$_REQUEST['ievparceria']}'
					)
				RETURNING ievid
				";
		}
		else
		{
			$value = $_POST['hid_valoritem'];
			$sql = "
				INSERT INTO evento.itemevento
					(
						eveid,
						ievdescricao,
						tieid,
						ievvalordf,
						ievdatainclusao,
						ievstatus,
						ievparceria
					)
				VALUES
					(
						'{$_SESSION['evento']['eveid']}',
						'{$_POST['hid_descricao']}',
						'{$_POST['tipoitemevento'][0]}',
						'{$_POST['tipoValor']}',
						'now()', 
						'A',
						'{$_REQUEST['ievparceria']}'
					)
				RETURNING ievid
				";
		}
	 
		$ievid = $db->pegaUm( $sql );
		 
		for( $i = 1; $i <= count( $_POST['qtd'] ); $i++){
			//$value = ($value ? str_replace( "," , "." ,str_replace( "." , "" , $value ) ) : 0);
            
			$value  = $value ? $value : 0;
            $qtd    = $_POST['qtd'][$i] ? $_POST['qtd'][$i] : 0;
            
            $sql = "
                INSERT INTO evento.detalheitemevento(
                        ievid,
                        diedia,
                        diequantidade,
                        dievalorunitario,
                        diedatainclusao
                    )VALUES(
                        {$ievid},
                        {$i}, 
                        {$qtd},
                        '{$value}',
                        'now()'
                    );
            ";		 
			$ins = $db->executar( $sql );
		}
        $db->commit();
        $db->sucesso('principal/cadEventoInfra');
        
	}else{
		echo"<script>alert('Impossível inserir mais de 1 vez um mesmo item de evento.');\n </script>";
		echo"<script>window.location.href = 'evento.php?modulo=principal/cadEventoInfra&acao=A';</script>";
	}
}

$titulo_modulo = "Eventos";

if( $_SESSION['evento']['eveid'] != ''){ 
	$sql = "SELECT 
				ev.evetitulo,  
				ev.ungcod,
				ev.tpeid,			 
				ev.evedatainicio,		 
				ev.evedatafim,			 
				ev.eveemail,
                                ev.everespnome,
				ev.evenumeropi, 		 
				ev.evenumeroprocesso,   
				ev.evecustoprevisto, 
				ev.evequantidadedias,	 
				ev.evepublicoestimado,  
				ev.muncod, 				 
				ev.estuf, 				 
				ev.sevid,
				ev.eveurgente,
				u.ungdsc,
				ev.docid,
        		us.usunome,
				to_char(ev.evedatainclusao::date,'DD/MM/YYYY') AS evedatainclusao	 
			FROM 
				evento.evento AS ev
			LEFT JOIN evento.tipoevento AS te 
				ON te.tpeid = ev.tpeid
			LEFT JOIN 
				public.unidadegestora AS u ON ev.ungcod = u.ungcod
			LEFT JOIN seguranca.usuario AS us ON us.usucpf = ev.usucpf 
			WHERE
				ev.eveid = '".$_SESSION['evento']['eveid']."'	
			";
	
	$rsDadosEvento = $db->carregar( $sql );
	 
}


//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
monta_titulo( $titulo_modulo, 'Solicitar Pré-agendamento de eventos.' );
echo'<br>';


?>
<br> 
<head>
   <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script src="../includes/prototype.js"></script>
    <script src="../includes/entidades.js"></script>
    <script src="../includes/calendario.js"></script>
    <!--
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
    -->
</head>
<? 
$res = array( 0 => array ( "descricao" => "Lista",
						    "id" 		=> "4",
						    "link" 		=> "/evento/evento.php?modulo=inicio&acao=C&submod=evento"
				  		  ),
 				1 => array ( "descricao" => "Informações Básicas",
						    "id" 		=> "4",
						    "link" 		=> "/evento/evento.php?modulo=principal/cadEvento&acao=A"
				  		  )
			);	  
				
if( ( $rsDadosEvento[0]['sevid'] != 1) AND ( $rsDadosEvento[0]['sevid'] != '') )
{
	array_push($res,
					array ("descricao" => "Documentos Anexos",
							    "id"        => "3",
							    "link" 		=> "/evento/evento.php?modulo=principal/cadEventoAnexo&acao=A"
							   ),
		   			array ("descricao" => "Infraestrutura",
							    "id"		=> "2",
							    "link"		=> "/evento/evento.php?modulo=principal/cadEventoInfra&acao=A"
					  		   )
			  );
	
	$pflcod = pegaPerfil( $_SESSION['usucpf'] ); 
/*
	 if( ( $pflcod == PERFIL_SUPER_USUARIO) || ( $pflcod == PERFIL_SAA ) )
	 { 
			array_push($res,	
					array ("descricao" => "Hotéis",
						    "id"		=> "1",
						    "link"		=> "/evento/evento.php?modulo=principal/cadEventoHotel&acao=A"
				  		   ) 
				    );
	 }
*/
	
}



/* RETIRANDO ABA "Estrutura Orçamentária" conforme solicitado na demanda 209333 item 8
     if($_SESSION['evento']['eveid']){
	array_push($res,	
				  	array ("descricao" => "Estrutura Orçamentária",
						    "id"		=> "6",
						    "link"		=> "/evento/evento.php?modulo=principal/cadEstruturaOrcamentaria&acao=A"
				  		   )
					  );	
}
*/
if(mostraAbaDocOS($_SESSION['evento']['eveid'])){
	array_push($res,	
				  	array ("descricao" => "Ordem de Serviço",
						    "id"		=> "7",
						    "link"		=> "/evento/evento.php?modulo=principal/cadOrdemServico&acao=A"
				  		   )
					  );	
}

if(mostraAbaDocPagamento($_SESSION['evento']['eveid'])){
	array_push($res,	
				  	array ("descricao" => "Documento de Pagamento",
						    "id"		=> "5",
						    "link"		=> "/evento/evento.php?modulo=principal/cadDocPagamento&acao=A"
				  		   )
					  );	
}

if( ( $rsDadosEvento[0]['sevid'] != 1) AND ( $rsDadosEvento[0]['sevid'] != '') )
{
	array_push($res, 
			  	array ("descricao" => "Avaliação",
					    "id"		=> "0",
					    "link"		=> "/evento/evento.php?modulo=principal/avaliacaoEvento&acao=A"
			  		   )
				  );
	
}


echo montarAbasArray($res, $_REQUEST['org'] ? false : "/evento/evento.php?modulo=principal/cadEventoInfra&acao=A"); 
headEvento($rsDadosEvento[0]['evetitulo'],$rsDadosEvento[0]['everespnome'], $rsDadosEvento[0]['ungdsc'], $rsDadosEvento[0]['ungcod'], $rsDadosEvento[0]['eveurgente'], $rsDadosEvento[0]['evedatainclusao'], true);
echo '<br>';

include APPRAIZ . 'includes/Agrupador.php';
	

$sql = "SELECT
                			tieid as codigo, 
                			tieitem ||' - '|| tiedescricao as descricao, 
                			tieidpai
                		FROM 	
                			evento.tipoitemevento
                		WHERE 	
                			tiastatus = 'A'
                		AND 
                			tieidpai is null
                		ORDER BY
                			tieitem ASC 
                "; 
                $rsComboItem = $db->carregar( $sql );

foreach( $rsComboItem as $origem ){
	$i++;
	$destino[$i] = array("codigo" => $origem['codigo'], "descricao" => ltrim($origem['descricao'], 0));

	$sql = "SELECT
                	tieid as codigo, 
                	tieitem ||' - '|| tiedescricao as descricao,
                	tieidpai as pai,
                	replace(tieitem, '.', '')::numeric as teste
                FROM 	
                	evento.tipoitemevento
                WHERE 	
                	tiastatus = 'A'
                AND 
                	tieidpai = '{$origem['codigo']}'
                ORDER BY
                	teste ASC 
                "; 

    $rsComboItemFilho = $db->carregar( $sql );
	foreach($rsComboItemFilho as $filho){
		$i++;
		$destino[$i] = array("codigo" => $filho['codigo'], "descricao" => $filho['descricao']);
		
		$sql = "SELECT
	                			tieid as codigo, 
	                			tieitem ||' - '|| tiedescricao as descricao,
	                			tieidpai,
	                			replace(tieitem, '.', '')::numeric as teste
	                		FROM 	
	                			evento.tipoitemevento
	                		WHERE 	
	                			tiastatus = 'A'
	                		AND 
	                			tieidpai = '{$filho['codigo']}'
	                		ORDER BY
                				teste ASC 
			                "; 

		$rsComboItemNeto = $db->carregar( $sql );
		
		if($rsComboItemNeto){
			foreach($rsComboItemNeto as $neto){
				$i++;
				$destino[$i] = array("codigo" => $neto['codigo'], "descricao" => $neto['descricao']);
			}
		}
	}
}


//verifica permissão edição
$eventoPermissaoEdicao = eventoPermissaoEdicao();

?> 
  <form name = "formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
			<td class ="SubTituloDireita" align="right" >Busca:</td>
			<td>
				<input id="buscatipoitemevento" name="buscatipoitemevento" class="normal" style="width: 650px;" onkeydown="return movimentoComteclaagrup(event, '', this ,'tipoitemevento', '');" onkeyup="limitaConteudoAgrupadoragrup(this,'tipoitemevento', event,'' );" onfocus="this.value = '';"  type="text" title=""  value="" style="width: 200px;" /><br>


			<tr>
			<td class ="SubTituloDireita" align="right" >Selecione um ítem de evento:</td>
			<td>			
				<!-- select id="tipoitemevento" name="tipoitemevento[]" size="7" style="width:650px;" onkeyup="carregaTipoEvento(this.value, 'fixo','');"  onclick="carregaTipoEvento(this.value, 'fixo');"   onDblClick="moveSelectedOptions( document.getElementById( 'tipoitemevento' ), document.getElementById( '' ), true, '' );" class="combo campoEstilo" -->
				<select id="tipoitemevento" name="tipoitemevento[]" size="7" style="width:650px;" onclick="carregaTipoEvento(this.value, 'fixo');"  class="combo campoEstilo">
					<?php 
						foreach($destino as $option){
						
							echo "<option value='{$option['codigo']}'>{$option['descricao']}</option>";
						}
					?>
				</select>				
			</td>
			
	<script type="text/javascript" language="javascript">
	limitarQuantidade( document.getElementById( 'tipoitemevento' ));
	//sortSelect( document.getElementById( 'tipoitemevento' ) );
	// TECLAS : 13 - enter  |  38 - para cima   |  40 - para baixo
	
	function movimentoComteclaagrup(tecla, tipo, local, campoOrigem, campoDestino){
		if(window.event){ // Se IE
			codigoTecla = tecla.keyCode;
		}else if(tecla.which){
			codigoTecla = tecla.which;
		}
		campo = local;
		campoBusca 	= document.getElementById( 'busca'+campoOrigem );
		charTecla 	= String.fromCharCode(codigoTecla);
		agrupador 	= document.getElementById(campoOrigem);
		selecao 	= document.getElementById(campoOrigem).selectedIndex;
		
			if(tecla.keyCode == 40){ // para baixo
				if(agrupador.selectedIndex == -1){	
					agrupador[0].selected = true;
				}else{
					for (i=0;i < agrupador.length; i++){
						if(agrupador[i].selected == true) {
							if(agrupador[i+1] != null){
								agrupador[i].selected = false;
								agrupador[i+1].selected = true;
								break;
							}else{
								agrupador[i].selected = false;
								break;
							}
						}
					}
				}
			}else if (tecla.keyCode == 13){ // tecla enter
				moveSelectedOptions( agrupador, '' , true, '' );
			}else if(tecla.keyCode == 38){ // para cima
				if(agrupador.selectedIndex == -1){	
					for (i=0;i < agrupador.length;i++){
							agrupador[i].selected = true;
							break;
					}
				}else{
					for (i=agrupador.length;i > 0;i--){
						if(agrupador[i] != null ){
							if(agrupador[i].selected == true) {
								if(agrupador[i-1] != null ){
									agrupador[i].selected = false;
									agrupador[i-1].selected = true;
									break;
								}else{
									break;
								}
							}
						}
					}
				}
				
			}
		return true;
	}
	
	var objsagrup 	= new Object;
		objsagrup.dados = {
					valor : new Array(), 
					texto : new Array() 
			  	 };
			  	 
	var agrupadoragrup =  document.getElementById('tipoitemevento'); 	 
	for (cont=0; cont < agrupadoragrup.length; cont++){
		objsagrup.dados.valor[cont] = agrupadoragrup[cont].value;
		objsagrup.dados.texto[cont] = agrupadoragrup[cont].text;
	}
	
	function limitaConteudoAgrupadoragrup(busca, nomeAgrupador, tecla, nomeAgrupador2){
		var selectAgrupador 		= document.getElementById(nomeAgrupador);
		var busca 					= busca.value;
		busca = busca.toLowerCase();
		if(tecla.keyCode != 40 && tecla.keyCode != 38 && tecla.keyCode != 13){
			if(busca != ''){ // SE BUSCAR ALGO
				for (i = selectAgrupador.length - 1; i>=0; i--) {
	      			selectAgrupador.remove(i);
				}
				for (cont = 0; cont<= objsagrup.dados.texto.length; cont++){
					if(objsagrup.dados.texto[cont]){
						banco = objsagrup.dados.texto[cont].toLowerCase();
						if(banco.indexOf(busca) != '-1'){
							var opcoes   = document.createElement('option');
							opcoes.text  = objsagrup.dados.texto[cont];
							opcoes.value = objsagrup.dados.valor[cont];
							try{
								selectAgrupador.add(opcoes,null); 
							}catch(ex){
								selectAgrupador.add(opcoes); // IE only
							}
						}
					}
				}
			}else{
				for (i = selectAgrupador.length - 1; i>=0; i--) {
	      			selectAgrupador.remove(i);
				}
				for (cont = 0; cont< objsagrup.dados.texto.length; cont++){
					var opcoes   = document.createElement('option');
					opcoes.text  = objsagrup.dados.texto[cont];
					opcoes.value = objsagrup.dados.valor[cont];
					try{
						selectAgrupador.add(opcoes,null); 
					}catch(ex){
						selectAgrupador.add(opcoes); // IE only
					}
				}
			}
		}
	}
	</script>
       <div id="itemAdd"></div>   
                </td>
                <td rowspan="5" style="background-color:#fafafa; color:#404040; width:1%" valign="top" align="left">
				<?php 
					wf_desenhaBarraNavegacao( $docid , array( '' => '' ) );			
				?>		
				</td>  
            </tr>  
<!--  
            <tr>
                <td class ="SubTituloDireita" align="right">Especificação do Ítem: </td>
                <td> 
                <?php
				$sql = "SELECT
                			tieid, 
                			tieitem,
                			tiedescricao, 
                			tieidpai
                		FROM 	
                			evento.tipoitemevento
                		WHERE 	
                			tiastatus = 'A'
                		AND 
                			tieidpai is null
                		ORDER BY
                			tieitem ASC 
                "; 
                $rsComboItem = $db->carregar( $sql );
                
                echo"<select  name=\"tipoitemevento\" onclick=\"return limparCampos();\" onchange=\"return carregaTipoEvento(this.value,'fixo');\"  id=\"tipoitemevento\"  class=\"CampoEstilo\" style=\"width: 500px;\"> ";
                echo"<option value=''>Selecione um ítem de evento...</option>"; 
               
                for( $i = 0; $i<count( $rsComboItem); $i++){ 
                	
                	if( $rsComboItem[$i]['tieid'] != '' ){
                		
	                	if( $rsComboItem[$i]['tieitemtemvalor'] == 'f'){
	                		$disable = "disabled = \"true\"";
	                		$style = "style=\"color: #191970;\"";
	                		$onchange = 'return false';
	                		$onclick  = 'return false;';
	                	}
	                	else
	                	{
	                		$disable = "";
	                		$style = "style=\"color: black;\"";
	                	} 
	                	echo"
	                		<option value = \"{$rsComboItem[$i]['tieid']}\" $disable $style $onchange $onclick >  $tab ".ltrim($rsComboItem[$i]['tieitem'], "0")."  -  {$rsComboItem[$i]['tiedescricao']} </div> </option>                		
	                	";
						
	                	$sql = "SELECT
	                			tieid, 
	                			tieitem,
	                			tiedescricao,
	                			tieidpai,
	                			replace(tieitem, '.', '')::numeric as teste
	                		FROM 	
	                			evento.tipoitemevento
	                		WHERE 	
	                			tiastatus = 'A'
	                		AND 
	                			tieidpai = '{$rsComboItem[$i]['tieid']}'
	                		ORDER BY
                				teste ASC 
			                "; 
                				
			                $rsComboItemFilho = $db->carregar( $sql );
	                		
			                for( $k = 0; $k<count($rsComboItemFilho);$k++){ 
			                	
			                	if($rsComboItemFilho[$k]['tieid'] != ''){ 
			                		
			                		if( $rsComboItemFilho[$k]['tieitemtemvalor'] == 'f'){
				                		$disable = "disabled = \"true\"";
				                		$style = "style=\"color: #191970;\"";
				                		$onchange = 'return false';
				                		$onclick  = 'return false;';
				                	}
				                	else
				                	{
				                		$disable = "";
				                		$style = "style=\"color: black;\"";
				                	} 
			                		$tab1 = "&nbsp;&nbsp;&nbsp;&nbsp;";
				                	echo"
			                		<option value = \"{$rsComboItemFilho[$k]['tieid']}\" $disable $style $onchange $onclick > $tab1 {$rsComboItemFilho[$k]['tieitem']}  -  {$rsComboItemFilho[$k]['tiedescricao']} </div> </option>                		
			                		";
			                		
			                		$sql = "SELECT
				                			tieid, 
				                			tieitem,
				                			tiedescricao,
				                			tieidpai,
				                			replace(tieitem, '.', '')::numeric as teste
				                		FROM 	
				                			evento.tipoitemevento
				                		WHERE 	
				                			tiastatus = 'A'
				                		AND 
				                			tieidpai = '{$rsComboItemFilho[$k]['tieid']}'
				                		ORDER BY
                							teste ASC 
						                "; 
				                			
					              	  $rsComboItemNeto = $db->carregar( $sql );
					              	  $tab1 = '';
					              	  
					              	   for( $j = 0; $j<count($rsComboItemNeto);$j++){
					              	   	
						                	if( $rsComboItemNeto[$j]['tieid'] != ''){
						                		
						                		if( $rsComboItemNeto[$j]['tieitemtemvalor'] == 'f'){
							                		$disable = "disabled = \"true\"";
							                		$style = "style=\"color: #191970;\"";
							                		$onchange = 'return false';
							                		$onclick  = 'return false;';
							                	}
							                	else{
							                		$disable = "";
							                		$style = "style=\"color: black;\"";
							                	} 
						                		$tab2 = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
							                	echo"
						                		<option value = \"{$rsComboItemNeto[$j]['tieid']}\" $disable $style $onchange $onclick > $tab2 {$rsComboItemNeto[$j]['tieitem']}  -  {$rsComboItemNeto[$j]['tiedescricao']} </div> </option>                		
						                		";
						                	}
						                	$tab2 = '';
						                }
				               		}
			                	}
                			}    
               			}
               			$perfis = arrayPerfil();
						 
						if( ( in_array(PERFIL_SAA, $perfis ) ) || ( in_array(PERFIL_SUPER_USUARIO, $perfis ) )|| ( in_array(PERFIL_DEMANDANTE, $perfis ) ) )
						{
	               			echo"
	                		<option value = \"AD\" style=\"background-color: #E0EEE0; color: #191970;\"> <b>  OUTROS  -</b>  ÍTENS ADICIONAIS  </div> </option>                		
	                		";
	                		echo "</select>";
						} 
                ?>  
                <div id="itemAdd"></div>   
                </td>
                <td rowspan="5" style="background-color:#fafafa; color:#404040; width:1%" valign="top" align="left">
				<?php 
					wf_desenhaBarraNavegacao( $docid , array( '' => '' ) );			
				?>		
				</td>  
            </tr>  
-->            
         	<tr>
                <td class ="SubTituloDireita" align="right">Parceria: </td>
                <td><input type="checkbox" id="ievparceria" name="ievparceria"></td> 
            </tr> 
         	<tr>
                <td class ="SubTituloDireita" align="right">Unidade de Medida: </td>
                <td id="unidadeMedida"> 
                </td> 
            </tr> 
            <tr>
        		<input type="hidden" name="hid_valoritem" id="hid_valoritem" value = 0>    
             	<input type="hidden" name="hid_tieitemtemcotacao" id="hid_tieitemtemcotacao" value = 0>       
             	<input type="hidden" name="hid_descricao" id="hid_descricao" value = 0>
             	<input type="hidden" name="hid_itemAdd" id="hid_itemAdd" value = 0>
             	<input type="hidden" name="hid_valordf" id="hid_valordf" value = 0>
             	<input type="hidden" name="hid_valoroutrauf" id="hid_valoroutrauf" value = 0>
             	<input type="hidden" name="update" id="update" value = "">
             	<input type="hidden" name="ievid" id="ievid" value = "">
            	<td class ="SubTituloDireita" align="right">
            		Valor: 
            	
            	</td>
            	<td id="tipoValor">  
            	 	<input type="radio" class="radio" name="tipoValor" id="tipoValordf" value="t" onchange="selecionaUf('df');" /><b>DF: </b><label name="lb_df" id="lb_df"></label> 
            	 	<br>
            	 	<input type="radio" class="radio" name="tipoValor" id="tipoValoroutro" value="f" onchange="selecionaUf('outra');"/><b>Outros: </b> <label name="lb_outra" id="lb_outra"> </label>
           		 </td>
           	</tr> 
           	<tr>
           	<td class ="subTituloDireita" >
           		Dias:
           		</td>
           		<td>
           		<div id="tb_dias">
           		<table width="200" border='0'> 
           		<tr>
           			<td> Dia:</td>
           			<td> Qtd:</td>
           			<td> Valor</td>
           		</tr>
           		<?php
           		$nDias = $rsDadosEvento[0]['evequantidadedias']; 
           		for( $i = 1; $i <= $nDias; $i++) {
           		?> 
           		<tr>
           			<td width="20%" >
           				Dia <?=$i;?>
           			</td>
           			<td  width="20%">
           				 <?php
						 echo"
						 <input type=\"text\" size=\"4\" class=\"normal\"name=\"qtd[$i]\"   onkeyup=\"this.value=mascaraglobal('###############',this.value); calcula(this.value, '$i');\" id=\"qtd[$i]\" value=''>
						 "; 
           				 ?> 
               		</td>
               		<td id="colunaValor[<?=$i;?>]" width="20%" >
           			<input type="hidden" name="hid_valoritem" id="hid_valoritem" value = 0>	 
           			</td>
               	</tr> 
           		<?      		 
           		} 
           		?>
           		<tr>
           			<td width="20%" >
           				 <b>Total</b>
           			</td>
           			<td  width="20%">
           				 
               		</td>
               		<td id="colunaValorTotal" width="20%" >
           			</td>
               	</tr>
           		</table> 
           		</div>
           		</td> 
           	</tr> 
           	<? if( !temPerfilEmpresa() ){ ?> 
           	<tr>
            	 <td  class ="SubTituloDireita" align="right"> 
            	 </td>
            	 <td>
	               <input type="hidden" name="action" value="0" id="action">
	               <input type="button" name="btnGravar" value="Gravar" id="btnGravar" onclick="validaForm('grava');">  
	               <input type="button" name="btnLimpar" value="Novo ítem" id="btnLimpar" onclick="limparCampos();">  
            	 </td> 
            </tr>	
            <? } ?>
            
            <table border="0" cellspacing="0" cellpadding="3" align="center" class="Tabela">
    <?
    		
    		if($_REQUEST['ordenaColuna'] == '2') $ordenaColuna = "ordemitem ".($_REQUEST['ordenaTipo'] ? $_REQUEST['ordenaTipo'] : 'ASC').",";
    		
            $sql = "SELECT
            				--'<center><a href=\"#\" onclick=\"javascript:carregaTipoEvento('|| te.tieid ||', ''cadastrado'');\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Excluir\"></a></center>' as acao,                       
                         	te.tieid,
            				te.tieitem,
            				case when strpos(te.tieitem, '.') = 0 then 
            					te.tieitem::numeric 
            				else 
            					(substr(te.tieitem, 1, strpos(te.tieitem, '.')-1) ||'.'|| replace(substr(te.tieitem, strpos(te.tieitem, '.')+1 ), '.',''))::numeric 
            				end as ordemitem,
                			te.tiedescricao,
                			te.tievalordf,
                			te.tievaloroutrauf,
                			ie.ievvalordf,
                			ie.ievid,
                			ie.ievdescricao,
                			ie.umeid,
                			CASE WHEN ie.ievparceria = true THEN '<center><img src=\"/imagens/check_p.gif\" border=0></center>'
                				 ELSE '<center><img src=\"/imagens/exclui_p.gif\" border=0></center>'                				 
                			END as ievparceria
                		FROM 	
                			evento.tipoitemevento as te
                		RIGHT OUTER JOIN evento.itemevento as ie 
                			ON ie.tieid = te.tieid  
                		WHERE
                			ie.eveid = '{$_SESSION['evento']['eveid']}'
                		AND 
							ie.ievstatus = 'A'
                		ORDER BY
                			$ordenaColuna
                			te.tieid ASC 
                			";

        $cabecalho = array( "Ação",
                            "Ítem",
                            "Descrição",
                            "Qtd Total",
                            "Valor Total");
        //$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '' );
        //dbg($sql,1);
        $dados = $db->carregar( $sql );
 
    if( $dados ){
    	
    	$tipoOrdena = $_REQUEST['ordenaTipo']; 
    	if($tipoOrdena == 'ASC') {
    			$tipoOrdena = "DESC";
    			$imgOrdena = "seta_baixo.png";
    	}
    	else{
    		$tipoOrdena = "ASC";
    		$imgOrdena = "seta_cima.png";
    	}
    	
    	?>
    	<table class="listagem" width="95%" align="center" border="0" cellpadding="2" cellspacing="0">
		    <input name="numero" value="" type="hidden">
		    <input name="ordemlista" value="" type="hidden">
		    <input name="ordemlistadir" value="" type="hidden">
		    <thead>
		    <tr>
		    <td class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" valign="top" align="" bgcolor="">
		    <strong>Ação</strong>
		    </td>
		    <td onclick="ordenaColuna('2','<?=$tipoOrdena?>');" class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" valign="top" align="" bgcolor="">
		    <strong>Ítem</strong> 
		    	<?if($_REQUEST['ordenaColuna']){?>
		    		<img src="/imagens/<?=$imgOrdena?>" title="Ordenar por Ítem" border="0">
	    		<?}?>
		    </td>
		    <td class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"  valign="top" align="" bgcolor="">
		    <strong>Descrição</strong>
		    </td>
		    <td class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"valign="top" align="">
		    <strong>Valor Unitário</strong>
		    </td>
		    <td class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"valign="top" align="">
		    <strong>Quantidade</strong>
		    </td>
		    <td class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"valign="top" align="">
		    <strong>Valor do Ítem</strong>
		    </td>
		    <td class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"valign="top" align="">
		    <strong>Nº dias</strong>
		    </td>
		     <td class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"valign="top" align="">
		    <strong>Valor Parcial/Total</strong>
		    </td>
		     <td class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"valign="top" align="">
		    <strong>Parceria</strong>
		    </td>
		    </tr> 
		    </thead>
		   <?
		   for( $i = 0; $i < count( $dados ); $i++)
		   {	   
			    ?> 
			    <tbody>
			    <tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';" bgcolor="">
				    <td title="Ação">
					    <center>
					    <?php
						if($dados[$i]['umeid'] == ''){
							$JsFunction = "javascript:carregaTipoEvento({$dados[$i]['tieid']}, 'cadastrado');";
						}
						else{
							$JsFunction = "javascript:carregaTipoEventoAdd({$dados[$i]['ievid']});";
						} 
					    ?>
					    <?
					    if(!$eventoPermissaoEdicao){

					    	if($dados[$i]['umeid'] == ''){?>
					    	
							     <a href="#" onclick="<?=$JsFunction?>">
							     <img src="/imagens/alterar.gif" title="Alterar" border="0"></a>
							     
							<?}?>
							
						     <a href="#" onclick="javascript:excluirItemEvento(<?=$dados[$i]['ievid']; ?>);">
						    <img src="/imagens/excluir.gif" title="Excluir" border="0"></a>
						    
						<?}?>
					    </center>
				    </td>
				    <td style="color: rgb(0, 102, 204);" title="Ítem" align="right">
				    	<?
						//if(!$dados[$i]['tieitem'])
				    	echo str_replace(",",".",($dados[$i]['tieitem'] ? $dados[$i]['tieitem'] : 'IA')); 
				    	?>
				    	<br>
				    </td>
				    <td title="Descrição">
				    	<?=($dados[$i]['tiedescricao'] ? $dados[$i]['tiedescricao'] : $dados[$i]['ievdescricao']); ?>
				    </td> 
				    <td title="Ítem" style="color: rgb(0, 102, 204);" align="right">
						<? /*
						if( $dados[$i]['umeid'] == '' )
						{ 
							if( $dados[$i]['ievvalordf'] == 't')
							{
								$valor =  number_format($dados[$i]['tievalordf'],2,",",".");
							}
							elseif( $dados[$i]['ievvalordf'] == 'f' )
							{
								$valor = number_format($dados[$i]['tievaloroutrauf'],2,",",".");
							}
							//echo'val = '.$valor.'<br>';
						}
						else
						{ 
							$s = "SELECT coalesce(d.dievalorunitario,0) FROM evento.detalheitemevento as d INNER JOIN evento.itemevento AS i ON i.ievid = d.ievid WHERE i.ievid = '{$dados[$i]['ievid']}' ";
							$valor = number_format( $db->pegaUm( $s ),2,",",".");
						}  
						//echo $valor;
						*/
						$query = "
								SELECT 
									d.diedia, 
									d.diequantidade,
									d.dievalorunitario
								FROM 
									evento.detalheitemevento AS d
								INNER JOIN evento.itemevento AS i
									ON i.ievid = d.ievid
								WHERE
									i.eveid = '{$_SESSION['evento']['eveid']}'
								AND
									d.ievid ='{$dados[$i]['ievid']}' 
								AND 
									i.ievstatus = 'A'
								LIMIT 1
							 ";
						
 							 
						$d = $db->pegaLinha( $query );
						
						echo $d['dievalorunitario']; 
						
						?>						
				    </td>
				    <td title="Descrição" align="center" style="color: rgb(0, 102, 204);" title="Valor Parcial/Total">
				    	<?
				    		$sqtd = "SELECT coalesce(sum(d.diequantidade),0) FROM evento.detalheitemevento as d INNER JOIN evento.itemevento AS i ON i.ievid = d.ievid WHERE i.ievid = '{$dados[$i]['ievid']}' ";
							$qtdItem =$db->pegaUm( $sqtd );
						 
							 echo $qtdItem;
							 $qtdItemTotal += $qtdItem;
				    	?>
				    </td> 
				    <td style="color: rgb(0, 102, 204);" align="center" title="Valor do Ítem">
				    	<?
						$query = "
								SELECT 
									d.diedia, 
									d.diequantidade,
									d.dievalorunitario
								FROM 
									evento.detalheitemevento AS d
								INNER JOIN evento.itemevento AS i
									ON i.ievid = d.ievid
								WHERE
									i.eveid = '{$_SESSION['evento']['eveid']}'
								AND
									d.ievid ='{$dados[$i]['ievid']}' 
								AND 
									i.ievstatus = 'A'
							 ";
						
 							 
						$rsDadosItem = $db->carregar( $query );
 
						 if( $dados[0]['ievid'] )
						 {
						 	 $diasUsados = 0;
							 for( $e = 0; $e<count( $rsDadosItem ); $e++ )
							 {
							 	$somaValores += $rsDadosItem[$e]['diequantidade'];
							 	if( $rsDadosItem[$e]['diequantidade'] > 0 ){
							 		$diasUsados ++;
							 		$valorNew = $rsDadosItem[$e]['dievalorunitario'];
							 	}	
							 } 
							// echo $valorNew.'fdsf';
							 $valorTotalItem = ( str_replace( ",", ".", $valorNew ) * str_replace( ",", ".", $somaValores ));
 
							 $val = $valorTotalItem ? $valorTotalItem: 0;
							 echo  number_format($val,2,",",".");
						 } 
						$somaValores = 0;
				    	?>
				    </td> 
				    <td title="Descrição" align="center" style="color: rgb(0, 102, 204);" title="Nº de dias">
				    	<?
							 echo $diasUsados;
				    	?>
				    </td> 	  
				    <td title="Descrição" align="center" style="color: rgb(0, 102, 204);" title="Valor Parcial/Total">
				    	<?
							$valorTotal += $valorTotalItem;
							echo number_format($valorTotal,2,",",".");
				    	?>
				    </td>
				    <td>
				    	<?=($dados[$i]['ievparceria'] ? $dados[$i]['ievparceria'] : $dados[$i]['ievparceria']); ?>
				    </td>   
				</tr> 
			<?
	 }
	    ?>
	    <tr>
		    <td  onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" valign="top" align="" bgcolor="">
		    <strong><B><center>TOTAL</center></B></strong>
		    </td>
		    <td  onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" valign="top" align="" bgcolor="">
		    <strong></strong>
		    </td>
		    <td  onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"  valign="top" align="" bgcolor="">
		    <strong></strong>
		    </td>
		    <td  onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"valign="top" align="">
		    <strong></strong>
		    </td>
		    <td  onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"valign="top" align="">
		    <strong><center><?							 
				echo $qtdItemTotal;
			?></center></strong>
		    </td>
		    <td  onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"valign="top" align="">
		    <strong></strong>
		    </td>
		    <td  onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"valign="top" align="">
		    <strong></strong>
		    </td>
		     <td onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" align="">
		    <strong><center>
		    <?							 
				echo number_format($valorTotal,2,",",".");
				if($_SESSION['evento']['eveid'] && $valorTotal != ""){
					$sql = "update evento.evento set evecustoprevisto = $valorTotal where eveid = {$_SESSION['evento']['eveid']}";
					$db->executar($sql);
					$db->commit($sql);
				}
			?></center>
		    </strong>
		    </td>
		    </tr> 
	    <table class="listagem" width="95%" align="center" border="0" cellpadding="2" cellspacing="0">
			    <tbody>
			    <tr bgcolor="#ffffff">
			    <td><b>Total de Registros: <?=count( $dados )?></b></td><td></td>
			    </tr>
			    </tbody>
		</table>
		<?
    }
    else
    {
    	?><table class="listagem" width="95%" align="center" border="0" cellpadding="2" cellspacing="0">
		    <tbody>
		    <tr bgcolor="#ffffff">
		    <td><b><center>Não foram encontrados registros</center></b></td><td></td>
		    </tr>
		    </tbody>
		    </table>
		 <?
    }
	?>
</table>
 </table>
 </form>
 <script>
top.selecionado 	= '';
top.selecTipoValor	= ''; 
function validaForm(tipo)
{ 
	if( tipo == 'grava' )
	{
		
		if( top.ufAdd != 1 )
		{
			var val_df 	  = document.getElementById('hid_valordf'); 	
		 	var val_outra = document.getElementById('hid_valoroutrauf');
		 	var tipoitemevento     = document.getElementById('tipoitemevento');
		 			 	
		 	if( tipoitemevento.value == '' )
		 	{
		 		alert('Você deve selecionar um ítem de evento.');
	 			return false;
		 	}
		 	
		 	if( top.selecTipoValor != 't' )
		 	{
		 		alert('Você deve preencher um tipo de valor');
	 			return false;
		 	}	 
 		}
 		
 		if( top.ufAdd == 1 )
 		{
 			var varAdd = document.getElementById('valor_cotado_add');
 			var umeid = document.getElementById('umeid');
 			if( varAdd.value == '' )
 			{
 				alert('Você deve preencher um valor para o campo selecionado.');
 				return false;
 			}
 			if( umeid.value == '' )
 			{
 				alert('Você deve selecionar uma unidade de medida para o ítem adicional.');
 				return false;
 			}
 		} 
		document.getElementById('action').value = tipo;
		document.formulario.submit();	
	}
}

function ordenaColuna(coluna, tipo)
{
	location.href='evento.php?modulo=principal/cadEventoInfra&acao=A&ordenaColuna='+coluna+'&ordenaTipo='+tipo;	
}
 
function calcula( qtd, dia )
{ 
 	 <?
	 if( $nDias != '' )
	 {
	 	 ?>	
		 var numCampos 	  = Number(<?= $nDias; ?>);
		 <?
	 }
 	 ?>	 
 	 
 	 var valorUnitario 	 = Number( document.getElementById('hid_valoritem').value );  
	 
	 var valorDia = '';
	 for( var i= 1; i<= numCampos; i++ )
	 {
	 	 valorDia   = Number( valorUnitario * Number(qtd) );
  	 
	 	 if( isNaN( valorDia ) )
	  	 { 
	  	 	 colunaValor 				= document.getElementById('colunaValor['+dia+']'); 
		 	 colunaValor.innerHTML	    = 0; 
		 }
		 else
		 {
		 	 colunaValor		   = document.getElementById('colunaValor['+dia+']'); 
		 	 colunaValor.innerHTML = float2moeda(valorDia); 
		 }		
	 }  
	 calculaTotal(numCampos);
} 
 
function calculaTudo()
 { 
 	 <?
	 if( $nDias != '' )
	 {
	 	 ?>	
		 var numCampos 	  = Number(<?= $nDias; ?>);
		 <?
	 }	  
	 ?>	
	 var qtd = 0; 
	 var valorUnitario 	 = Number( document.getElementById('hid_valoritem').value ); 
	 
	 var valorDia = ''; 
	 for( var i= 1; i<= numCampos; i++ )
	 {
	 
	 	 var qtd = document.getElementById('qtd['+i+']').value;
	 	 valorDia    = Number( valorUnitario * Number(qtd) )
	   
	  	 if( isNaN( valorDia ) )
	  	 { 
	  	 	 colunaValor      			= document.getElementById('colunaValor['+i+']'); 
		 	 colunaValor.innerHTML 		= 0; 
		 }
		 else
		 {
		 	 colunaValor 				= document.getElementById('colunaValor['+i+']'); 
		 	 colunaValor.innerHTML 		= float2moeda(valorDia); 
		 } 
	 }   
	 calculaTotal(numCampos);
}

function calculaTotal(numCampos)
{
	 var colunaValorTotal = document.getElementById('colunaValorTotal');
	 var valorTotal =0; 
	 var valorParcial =0;
	 for( var i= 1; i<= numCampos; i++ )
	 {  
	  	 colunaValue  = document.getElementById('colunaValor['+i+']'); 
	  	 valorParcial = colunaValue.innerHTML; 
	  	 valorParcial = valorParcial.replace(".","");
		 valorParcial = valorParcial.replace(",",".");
		 valorTotal  += Number( valorParcial );
	 }   
	 
	 colunaValorTotal.innerHTML = float2moeda(valorTotal);		
}

function carregaTipoEvento( id , tipo)
{
	if(id == 'AD'){
		return cadastraItensAdd();
	}
	
 	var inputIevid 	 	 = document.getElementById('ievid');
 	var hid_valoritem 	 = document.getElementById('hid_valoritem');
 	var tipoitemevento 	 = document.getElementById('tipoitemevento');
 	var hid_valordf 	 = document.getElementById('hid_valordf'); 	
 	var hid_valoroutrauf = document.getElementById('hid_valoroutrauf');
 	var lb_df 			 = document.getElementById('lb_df'); 
 	var lb_outra 		 = document.getElementById('lb_outra');
 	var hid_update		 = document.getElementById('update'); 
 	var hid_descricao 	 = document.getElementById('hid_descricao'); 	
 	var hid_tieitemtemcotacao 	 = document.getElementById('hid_tieitemtemcotacao'); 	
 	
 	
 	var unidadeMedida = document.getElementById('unidadeMedida');
 	var req = new Ajax.Request('evento.php?modulo=principal/cadEventoInfra&acao=A&tipo='+tipo, {
							        method:     'post',
							        parameters: '&ajaxitem=' + id,							         
							        onComplete: function (res)
							        {			
										arrResponse = res.responseText.split("_");
										
										var parceria = document.getElementById('ievparceria');
										
										if(arrResponse[9] == 't'){											
											parceria.checked = 'checked';											
										} else {
											parceria.checked = '';
										}
								 		 
										if( tipo == 'cadastrado') 
										{
											var tdDias = '';
											var arrDias = arrResponse[7].split(","); 
											<?
											 if( $nDias != '' )
											 {
											 	 ?>	
												 var nDias 	  = Number(<?= $nDias; ?>);
												 <?
											 }
										 	 ?> 
											tdDias  = "<table width=\"200\" border='0'> "; 
											tdDias += "<tr><td> Dia:</td><td> Qtd:</td><td> Valor</td></tr>";  
           									
							           		for( var i = 1; i <= nDias; i++)
							           		{  
							           			if( !arrDias[i-1] )
							           			{
							           				arrDias[i-1] = 0;
							           			}												
								           		tdDias += "<tr> <td width=\"20%\" > Dia "+i+"</td>";
								           		tdDias +=" <td  width=\"20%\"> <input type=\"text\" size=\"4\" class=\"normal\"name=\"qtd["+i+"]\" id=\"qtd["+i+"]\" value="+arrDias[i-1]+" onkeyup=\"this.value=mascaraglobal('###############',this.value); calcula(this.value, '"+i+"');\"> </td>";
												tdDias +="<td id=\"colunaValor["+i+"]\" width=\"20%\" >";			
								           		tdDias +="<input type=\"hidden\" name=\"hid_valoritem\" id=\"hid_valoritem\" value = 0> </td></tr>"; 
							               	}
							               	
								            tdDias +="	<tr><td width=\"20%\" > <b>Total</b> </td><td  width=\"20%\"></td><td id=\"colunaValorTotal\" width=\"20%\" ></td></tr>";
							            	tdDias += "</table>";
							            	  
							               	var tb_dias  		= document.getElementById('tb_dias');
									 	   	tb_dias.innerHTML	= tdDias; 	
										}	
										var tieid			  = arrResponse[0];
									    var umedescricao 	  = arrResponse[1];
									 	var tievalordf 	 	  = arrResponse[2];
										var tievaloroutrauf   = arrResponse[3];
										var tiedescricao      = arrResponse[4];
										var tieitemtemcotacao = arrResponse[5];
										var ievvalordf 	      = arrResponse[6];
										var update  	      = arrResponse[8];
										var ievid 	 	      = arrResponse[8];
										
							 			if( ievvalordf == 't')
							 			{   
							 				document.getElementById('tipoValordf').checked = true;
							 				hid_valoritem.value                            = tievalordf; 
							 				top.selecTipoValor							   = 't'; 
							 			}
							 			else if( ievvalordf == 'f' )
							 			{   
							 				document.getElementById('tipoValoroutro').checked = true;
							 				hid_valoritem.value								  = tievaloroutrauf;
							 				top.selecTipoValor							   = 't';  
							 			}
							 			
										unidadeMedida.innerHTML = '<b>'+umedescricao+'</b>';
										lb_df.value 		= '<b> R$: '+float2moeda(tievalordf)+'</b>'; 
										lb_outra.value 		= '<b> R$: '+float2moeda(tievaloroutrauf)+'</b>'; 
										<?
										$perfis = arrayPerfil();
						 
										if( ( in_array(PERFIL_SAA, $perfis ) ) || ( in_array(PERFIL_SUPER_USUARIO, $perfis ) ) )
										{
											?>
											 lb_df.innerHTML 	= '<b> R$: '+float2moeda(tievalordf)+'</b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  <input type=\'button\' value=\'Alterar\' name=\'abre_df\' id=\'abre_df\' onclick = \'abrirCamposValor("df");\'> '; 
											 lb_outra.innerHTML  = '<b> R$: '+float2moeda(tievaloroutrauf)+'</b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type=\'button\' value=\'Alterar\' name=\'abre_outra\' id=\'abre_outra\' onclick = \'abrirCamposValor("outra");\'> '; 
										
											<?php
										}else
										{	
											?>
											lb_df.innerHTML 	= '<b> R$: '+float2moeda(tievalordf)+'</b>'; 
											lb_outra.innerHTML  = '<b> R$: '+float2moeda(tievaloroutrauf)+'</b> '; 
											<?
										}
						                ?> 
										hid_descricao.value 		= tiedescricao;
										hid_tieitemtemcotacao.value = tieitemtemcotacao; 
										hid_valordf.value 			= tievalordf;
										hid_valoroutrauf.value 		= tievaloroutrauf;
										tipoitemevento.value		= tieid;
										if(update){
											hid_update.value		= update;
										}											
										inputIevid.value 			= ievid;
										calculaTudo();			 
										top.selecionado = 'v';
							        } 
							  });
}
 
 function float2moeda(num) {
   x = 0;
   if(num<0) {
      num = Math.abs(num);
      x = 1;
   }
   if(isNaN(num)) num = "0";
      cents = Math.floor((num*100+0.5)%100);
   num = Math.floor((num*100+0.5)/100).toString();
   if(cents < 10) cents = "0" + cents;
      for (var i = 0; i < Math.floor((num.length-(1+i))/3); i++)
         num = num.substring(0,num.length-(4*i+3))+'.'
               +num.substring(num.length-(4*i+3));
    ret = num + ',' + cents;
    if (x == 1) ret = ' - ' + ret;
    return ret;
}

function limparCampos()
{ 
	var numCampos 			  = document.formulario.length;
	var lb_df 			 	  = document.getElementById('lb_df'); 
 	var lb_outra 		 	  = document.getElementById('lb_outra'); 
 	var hid_descricao 	 	  = document.getElementById('hid_descricao'); 	
 	var hid_tieitemtemcotacao = document.getElementById('hid_tieitemtemcotacao'); 	
 	var colunaValorTotal 	  = document.getElementById('colunaValorTotal');
 	var unidadeMedida 		  = document.getElementById('unidadeMedida');
	var val_df				  = document.getElementById('tipoValordf');
	var val_outra			  = document.getElementById('tipoValoroutro');
	var tipoitemevento		  = document.getElementById('tipoitemevento');
	var divItemAdd 		 	  = document.getElementById('itemAdd');
	for( var i= 0; i< numCampos; i++)
	{
		if( (document.formulario[i].id != 'btnGravar') && ( document.formulario[i].id != 'btnLimpar') && ( document.formulario[i].id != 'tipoValordf') && ( document.formulario[i].id != 'tipoValoroutro'))
		{
			document.formulario[i].value = '';
		}
	}
	 <?
	 if( $nDias != '' )
	 {
	 	 ?>	
		 var numCampos 	  = Number(<?= $nDias; ?>);
		 <?
	 }
	 ?>	
 	for( var i = 1; i<= numCampos; i++)
 	{
 		document.getElementById('colunaValor['+i+']').innerHTML = '';
 	}
 	
	lb_outra.innerHTML 		   = '';
	lb_df.innerHTML    		   = '';
	colunaValorTotal.innerHTML = ''; 
	divItemAdd.innerHTML	   = '';
	unidadeMedida.innerHTML	   = '';
	val_df.checked 			   = false;
	val_outra.checked 		   = false; 
	tipoitemevento.disabled    = false;
	
}

function limparCamposPos()
{ 
	var numCampos 			  = document.formulario.length;
	var lb_df 			 	  = document.getElementById('lb_df'); 
 	var lb_outra 		 	  = document.getElementById('lb_outra'); 
 	var hid_descricao 	 	  = document.getElementById('hid_descricao'); 	
 	var hid_tieitemtemcotacao = document.getElementById('hid_tieitemtemcotacao'); 	
 	var colunaValorTotal 	  = document.getElementById('colunaValorTotal');
 	var unidadeMedida 		  = document.getElementById('unidadeMedida');
	var val_df				  = document.getElementById('tipoValordf');
	var val_outra			  = document.getElementById('tipoValoroutro');
	var tipoitemevento		  = document.getElementById('tipoitemevento');
	var divItemAdd 		 	  = document.getElementById('itemAdd');
	
	for( var i= 0; i< numCampos; i++)
	{
		if( (document.formulario[i].id != 'btnGravar') && ( document.formulario[i].id != 'btnLimpar') && ( document.formulario[i].id != 'tipoValordf')&& ( document.formulario[i].id != 'tipoValoroutro'))
		{
			document.formulario[i].value = '';
		}
	}
	 <?
	 if( $nDias != '' )
	 {
	 	 ?>	
		 var numCampos 	  = Number(<?= $nDias; ?>);
		 <?
	 }
	 ?>	
 	for( var i = 1; i<= numCampos; i++)
 	{
 		document.getElementById('colunaValor['+i+']').innerHTML = '';
 	}
 	
	lb_outra.innerHTML 		   = '';
	lb_df.innerHTML    		   = '';
	colunaValorTotal.innerHTML = ''; 
	tipoitemevento.disabled = false;
}

function selecionaUf(uf)
{ 
	var hid_valoritem 	 = document.getElementById('hid_valoritem');
	var hid_valordf 	 = document.getElementById('hid_valordf'); 	
 	var hid_valoroutrauf = document.getElementById('hid_valoroutrauf');
 	
	if( uf == 'df' )
	{ 
		hid_valoritem.value = hid_valordf.value;
	}
	else if( uf == 'outra' )
	{ 
		hid_valoritem.value = hid_valoroutrauf.value;
	}	
	 
	top.ufAdd = 0;
	top.selecTipoValor = 't';
	calculaTudo();			
}

function abrirCamposValor(tipo)
{
 	var lb_df 			 = document.getElementById('lb_df'); 
 	var lb_outra 		 = document.getElementById('lb_outra');
 	var unidadeMedida 	 = document.getElementById('unidadeMedida');
 
 	if( tipo == 'df'){
 		var content_df    = "R$ <input type=\"text\" size=\"15\" onkeyup=\"this.value=mascaraglobal('#.###.###,##',this.value);\" class=\"normal\" name=\"valor_cotado_df\" id=\"valor_cotado_df\" value=\"\"> &nbsp;&nbsp;<input type=\"button\" value=\"salva\" name=\"salva_df\" id=\"salva_df\" onclick =\"salvarValorAjax('tievalordf');\"> &nbsp;<input type=\"button\" value=\"Cancelar\" name=\"cancela_df\" id=\"cancela_df\" onclick=\"cancelar('df');\">";
 		lb_df.innerHTML	   = content_df;
 	}
 	else if( tipo == 'outra'){
  		var content_outra = "R$ <input type=\"text\" size=\"15\" onkeyup=\"this.value=mascaraglobal('#.###.###,##',this.value);\" class=\"normal\" name=\"valor_cotado_outra\" id=\"valor_cotado_outra\" value=\"\"> &nbsp;&nbsp;<input type=\"button\" value=\"salva\" name=\"salva_outra\" id=\"salva_outra\" onclick =\"salvarValorAjax('tievaloroutrauf');\">  &nbsp;<input type=\"button\" value=\"Cancelar\" name=\"cancela_outra\" id=\"cancela_outra\" onclick=\"cancelar('outra');\">";
 		lb_outra.innerHTML = content_outra;
 	}  
}

function cancelar(uf)
{
	var hid_valordf = '';
	var hid_valoroutrauf = '';
 	var hid_valordf 	 = document.getElementById('hid_valordf').value; 	
 	var hid_valoroutrauf = document.getElementById('hid_valoroutrauf').value;
 	var lb_df 			 = document.getElementById('lb_df'); 
 	var lb_outra 		 = document.getElementById('lb_outra');
 	var uf = uf; 
 	if( uf == 'df')
 	{ 
 		var content_button  = '<input type="hidden" /><b>R$'+float2moeda(hid_valordf)+
 							  '</b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type=\'button\' value=\'Alterar\' name="\'df\'" id="\'df\'"  onclick="abrirCamposValor(\''+uf+'\');"></b> ';
 		//lb_df.innerHTML = ""; 
 		lb_df.innerHTML = content_button;
 	}
 	else if( uf == 'outra' ) 
 	{
 		//var content_button  = "<b>R$: "+float2moeda(hid_valoroutrauf)+"</b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  <input type=\"button\" value=\"Alterar\" name=\"outra\" id=\"outra\" onclick=\"abrirCamposValor('outra');\">";
 		var content_button  = '<input type="hidden" /><b>R$'+float2moeda(hid_valordf)+'</b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type=\'button\' value=\'Alterar\' name=\'outra\' id=\'outra\' onclick="abrirCamposValor(\''+uf+'\');"></b> ';
 		
 		lb_outra.innerHTML = content_button; 
 	} 
}

function salvarValorAjax(tipo)
{
	 var tipoitemevento 	 = document.getElementById('tipoitemevento').value;
	 var lb_df 			 = document.getElementById('lb_df'); 
 	 var lb_outra 		 = document.getElementById('lb_outra'); 
 	 
	 if( tipo == 'tievalordf' )
	 {
 	 	var valorUnitario 	 = document.getElementById('valor_cotado_df').value;
 	    valorUnitario = valorUnitario.replace(".","");
	    valorUnitario = valorUnitario.replace(",",".");
 	 	var label            = lb_df;
 	 	var uf				 = "df"; 
 	 	var hid				 = document.getElementById('hid_valordf'); 	
 		if( valorUnitario == '' )
 		{
 			alert('Você deve preencher um valor para salvar.');
 			return false;
 		}
 	 }
 	 else if(tipo == 'tievaloroutrauf')
 	 {
	 	 var valorUnitario 	 = document.getElementById('valor_cotado_outra').value;  
	 	 valorUnitario = valorUnitario.replace(".","");
	     valorUnitario = valorUnitario.replace(",",".");
	 	 var label           = lb_outra;
	 	 var uf				 = "outra";
	 	 var hid			 = document.getElementById('hid_valoroutrauf');
	 	 if( valorUnitario == '' )
	 	 {
	 		alert('Você deve preencher um valor para salvar.');
	 		return false;
	 	 }
     } 
	 
     var req = new Ajax.Request('evento.php?modulo=principal/cadEventoInfra&acao=A&salvaTipo='+tipo+'&tieid='+tipoitemevento, {
							        method:     'post',
							        parameters: '&valorAjax=' + valorUnitario,							         
							        onComplete: function (res)
							        {							        	
										resp = res.responseText; 
									 
										hid.value = resp;  
										label.innerHTML = '<b>R$'+float2moeda(resp)+'&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;  <input type=\'button\' value=\'Alterar\' name=\'abre_df\' id=\'abre_df\' onclick = \'abrirCamposValor("'+uf+'");\'></b> ';						        
										extraiScript(res.responseText)
										
							        }
    });
}

function extraiScript(texto)
{ 
	var ini, pos_src, fim, codigo;  
	var objScript = null;  
	ini = texto.indexOf('<script', 0)  
	while (ini!=-1){  
		var objScript = document.createElement("script");  
		//Busca se tem algum src a partir do inicio do script  
		pos_src = texto.indexOf(' src', ini)  
		ini = texto.indexOf('>', ini) + 1;

		//Verifica se este e um bloco de script ou include para um arquivo de scripts  
		if (pos_src < ini && pos_src >=0){//Se encontrou um "src" dentro da tag script, esta e um include de um arquivo script  
			//Marca como sendo o inicio do nome do arquivo para depois do src  
			ini = pos_src + 4;  
			//Procura pelo ponto do nome da extencao do arquivo e marca para depois dele  
			fim = texto.indexOf('.', ini)+4;  
			//Pega o nome do arquivo  
			codigo = texto.substring(ini,fim);  
			//Elimina do nome do arquivo os caracteres que possam ter sido pegos por engano  
			codigo = codigo.replace("=","").replace(" ","").replace("\"","").replace("\"","").replace("\'","").replace("\'","").replace(">","");  
			// Adiciona o arquivo de script ao objeto que sera adicionado ao documento  
			objScript.src = codigo;  
		}else{
		//Se nao encontrou um "src" dentro da tag script, esta e um bloco de codigo script  
			// Procura o final do script
			fim = texto.indexOf('</script', ini);  
			// Extrai apenas o script
			codigo = texto.substring(ini,fim);  
			// Adiciona o bloco de script ao objeto que sera adicionado ao documento  
			objScript.text = codigo;
		} 
		//Adiciona o script ao documento  
		document.body.appendChild(objScript);
		
		// Procura a proxima tag de <script>  
		ini = texto.indexOf('<script', fim);
		
		//Limpa o objeto de script  
		objScript = null;  
	}  
} 

function excluirItemEvento(valor)
{ 
	if(confirm("Deseja excluir este item?")){
		if( valor != ''){
		 
			return location.href='evento.php?modulo=principal/cadEventoInfra&acao=A&excluir='+valor; 
		} 
	} else {
		return false;
	}
}	

function cadastraItensAdd()
{ 
	var itemAdd 		 = prompt( 'Título do ítem adicional:', 'Novo ítem' );
	var divItemAdd 		 = document.getElementById('itemAdd');
	var hid_itemAdd 	 = document.getElementById('hid_itemAdd');
	var tipoitemevento   = document.getElementById('tipoitemevento');
	var unidadeMedida    = document.getElementById('unidadeMedida');
	var tipoValor		 = document.getElementById('tipoValor');
	var hid_valordf      = '';
	var hid_valoroutrauf = '';
 
	if(itemAdd)
	{
		var req = new Ajax.Request('evento.php?modulo=principal/cadEventoInfra&acao=A&montaComboUnidade=S', 
		{
							        method:     'post',
							        parameters: '',							         
							        onComplete: function (res)
							        {							        	
										resp = res.responseText; 
										unidadeMedida.innerHTML = resp;  
									
										divItemAdd.innerHTML 	= '<B><br>'+itemAdd+'</B>';
										tipoitemevento.disabled = true;
					 			 		
							 		    var content_tipo_valor   = '<input type="radio" class="radio" name="tipoValor" id="tipoValordf" value="t" onclick=\'selecionaUfAdd("df"); \' /><b>DF: R$</b><label name="lb_df" id="lb_df"></label>'+ 
											            	 	   '<br>'+
											            	 	   '<input type="radio" class="radio" name="tipoValor" id="tipoValoroutro" value="f" onclick=\'selecionaUfAdd("outra"); /><b>Outros: R$</b> <label name="lb_outra" id="lb_outra"> </label>'
								 		
								 		tipoValor.innerHTML = content_tipo_valor;
 								 		
 								 		hid_itemAdd.value = itemAdd;
							        }
   		}); 
	} 
}
 
function selecionaUfAdd(uf)
{
	top.ufAdd = 1;
	var hid_valordf 	 = document.getElementById('hid_valordf').value; 	
 	var hid_valoroutrauf = document.getElementById('hid_valoroutrauf').value;
 	var hid_valoritem    = document.getElementById('hid_valoritem').value;
 	var lb_df 			 = document.getElementById('lb_df'); 
 	var lb_outra 		 = document.getElementById('lb_outra');
 	
 	if( uf == 'df')
 	{
 		label = lb_df;
 		label_close = lb_outra;
 	}
 	else if( uf == 'outra')
 	{
 		label = lb_outra;
 		label_close = lb_df;
 	} 
    //var content_button = '</b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type=\'button\' value=\'Alterar\' name="\'df\'" id="\'df\'" onclick="abrirCamposValor(\''+uf+'\');"></b> ';
    var content_outra = "<input type=\"text\" size=\"15\" onkeyup=\"this.value=mascaraglobal('#.###.###,##',this.value);\" class=\"normal\" name=\"valor_cotado_add\" id=\"valor_cotado_add\" value=\"\"><img src=\"../imagens/obrig.gif\" title=\"Indica campo obrigatorio.\" border=\"0\">";
    label.innerHTML	   = content_outra;    
    
    var content_close = '';
    label_close.innerHTML = content_close;
} 
calculaTudo();
 </script>
 
<?php 
//verifica permissao edição
echo $eventoPermissaoEdicao;
?>