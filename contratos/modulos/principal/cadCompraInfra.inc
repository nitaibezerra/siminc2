<?php
//include APPRAIZ . "includes/classes/dateTime.inc";

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
$coaid = pegaCoaid($_SESSION['copid'], $_SESSION['unidade']);


$sql = "select 
			coeid
	     FROM evento.coenderecoentrega  ee 
	     	inner join evento.coadesao a on a.coaid = ee.coaid
	     where a.copid = ". $_SESSION['copid'] ." and a.usgid = ". $_SESSION['unidade'];
$boEndereco = $db->pegaUm($sql);

if($boEndereco){
	$boAlterar = permissaoAlterar($coaid);
}

$sql = "SELECT to_char(copdatalimite::date,'DD/MM/YYYY') AS datalimite FROM evento.coprocesso WHERE copid = {$_SESSION['copid']}";
$data = $db->pegaUm($sql);

$dataAtual = date('d/m/Y'); 
$obData = new Data();
$retorno = $obData->diferencaEntreDatas(  $dataAtual, $data, 'maiorDataBolean', null, 'dd/mm/yyyy');

if( $_REQUEST['excluir'] != '' ){ 
	$sql = "DELETE FROM evento.codemandaitem WHERE cotid = {$_REQUEST['excluir']} AND coaid = {$coaid}";
	$up  = $db->executar( $sql );
	$db->commit(); 
	redirecionar( $_REQUEST['modulo'], $_REQUEST['acao'] );
}
 
if( $_POST['ajaxitem'] ){
	if( $_GET['tipo'] == 'fixo'){
		$coiid = $_POST['ajaxitem'];
		$sqlCoiid = "select cotid from evento.coitemprocesso where coiid = {$coiid}"; 
		$cotid  = $db->pegaUm( $sqlCoiid ); 
		 
		$sql = "		SELECT
                			it.coiid, 
                			it.coidsc,
                			it.umeid,
                			un.umedescricao,
                			it.coivlrreferenciamin,
                			it.coivlrreferenciamax, 
                			it.coiespecificacao 
                		FROM 	
                			evento.coitem AS it
                		LEFT JOIN
                			evento.unidademedida AS un ON it.umeid = un.umeid
                		WHERE it.coiid = '{$coiid}' 
                		 ";
		
		$rsAjax = $db->carregar( $sql );
		$sqlQtd = "select distinct c.coeid, c.coaid, c.coenddsc from evento.coenderecoentrega as c
								left join evento.codemandaitem as coi ON coi.coeid = c.coeid AND coi.coaid = c.coaid
								where c.coaid = {$coaid}
		";
		 
		
		$d = $db->carregar( $sqlQtd );
 		if( $d ){
 			$arr = array();
 			for( $i = 0; $i<count($d); $i++){ 
 				if(  $d[$i]['coeid'] != ''){ 
 					$arr[] =  $d[$i]['cdiqtde'];
 					$qtde  = implode(",", $arr);
 					$arr2[]   =  $d[$i]['coenddsc'];
 					$coenddsc = implode(",", $arr2);
 					$arr3[] =  $d[$i]['coeid'];
 					$coeid  = implode(",", $arr3);
 				}
 			}
 		}   
  		 
		$resp = $coiid.'_'.$rsAjax[0]['coidsc'].'_'.$rsAjax[0]['umeid'].'_'.$rsAjax[0]['umedescricao'].'_'.$rsAjax[0]['coivlrreferenciamin'].'_'.$rsAjax[0]['coivlrreferenciamax'].'_'.$rsAjax[0]['coiespecificacao'].'_'.$cotid.'_'.$coenddsc.'_'.$coeid;
		echo $resp;
		die();
	}
}

if( $_POST['ajaxitemCadastrado']){
		global $db; 
		$sqlCoiid = "select coiid from evento.coitemprocesso where cotid = {$_POST['ajaxitemCadastrado']}"; 
		$coiid  = $db->pegaUm( $sqlCoiid ); 
		$sql = "		SELECT
                			it.coiid, 
                			it.coidsc,
                			it.umeid,
                			un.umedescricao,
                			it.coivlrreferenciamin,
                			it.coivlrreferenciamax, 
                			it.coiespecificacao 
                		FROM 	
                			evento.coitem AS it
                		LEFT JOIN
                			evento.unidademedida AS un ON it.umeid = un.umeid
                		WHERE it.coiid = '{$coiid}' 
                		 "; 
		$rsAjax = $db->carregar( $sql );
		
		$sqlQtd = "select c.coaid, c.coeid, c.coenddsc, coi.cdiqtde from evento.coenderecoentrega as c
								left join evento.codemandaitem as coi ON coi.coeid = c.coeid AND coi.coaid = c.coaid
								where c.coaid = {$coaid} and coi.cotid = {$_POST['ajaxitemCadastrado']}";
		
		$d = $db->carregar( $sqlQtd );
 		if( $d ){
 			$arr = array();
 			for( $i = 0; $i<count($d); $i++){ 
 				if(  $d[$i]['cdiqtde'] != ''){ 
 					$arr[] =  $d[$i]['cdiqtde'];
 					$qtde = implode(",", $arr);
 					$arr2[] =  $d[$i]['coenddsc'];
 					$coenddsc = implode(",", $arr2);
 					$arr3[] =  $d[$i]['coeid'];
 					$coeid  = implode(",", $arr3);
 				}
 			}
 		}    
 		 
		$resp = $rsAjax[0]['coiid'].'_'.$rsAjax[0]['coidsc'].'_'.$rsAjax[0]['umeid'].'_'.$rsAjax[0]['umedescricao'].'_'.$rsAjax[0]['coivlrreferenciamin'].'_'.$rsAjax[0]['coivlrreferenciamax'].'_'.$rsAjax[0]['coiespecificacao'].'_'.$qtde.'_'.$coenddsc.'_'.$coeid.'_'.$_POST['ajaxitemCadastrado'].'_'.$Coitems;
		echo $resp;
		die();
}
if( $_POST['action'] == 'grava' ){ 
	salva(); 
}
  
function salva(){
	global $db; 
	 
 	$docid = evtPegarDocCompra($_SESSION['coaid']);
   	$coaid = pegaCoaid($_SESSION['copid'], $_SESSION['unidade']);
   	
   	if( $_POST['update'] == 't' ){
   		$sqlDel = "DELETE FROM evento.codemandaitem WHERE cotid = {$_POST['cotid']} AND coaid = {$coaid}";
   		$del = $db->executar( $sqlDel );
   		$db->commit(); 
   	}
   	
		for( $i = 0; $i < count( $_POST['coiqtde'] ); $i++){ 
			if( $_POST['coiqtde'][$i] == '' ){
				$coiqtde = 0;
			}else{
				$coiqtde = $_POST['coiqtde'][$i];
			}
			if( is_numeric($_POST['coeid'][$i]) && is_numeric($coiqtde) ){
				$sql = "
					INSERT INTO evento.codemandaitem
					(
						coeid,
						coaid,
						cdiqtde,
						cotid
					)
				VALUES
					(
						'{$_POST['coeid'][$i]}',
						{$coaid},
						'{$coiqtde}',
						'{$_POST['cotid']}' 
						 
					) 
				";
				$ins = $db->executar( $sql );				
			}
		}
		gravaGestor($coaid,$_SESSION['unidade'],$_POST['gestor']);
		$db->commit();
		$db->sucesso('principal/cadCompraInfra'); 
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
<body <?php if ($retorno == true){?>onload="alert('Adesão encerrada!');"<?php } ?>>
<form name = "formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    		<tr>
    		    <td class ="SubTituloDireita" align="right">Gestor da Ata: </td>
                <td>
    		 <?php
               
                $sql = "select distinct rpu.usucpf as codigo, 
							max(tcs.nu_matricula_siape) || ' - ' || usu.usunome as descricao
						from evento.usuarioresponsabilidade rpu
							 inner join seguranca.usuario usu on rpu.usucpf = usu.usucpf
							 inner join siape.tb_siape_cadastro_servidor tcs on rpu.usucpf = tcs.nu_cpf
						where rpu.usgid = ". $_SESSION['unidade']."	and rpustatus = 'A'
						group by rpu.usucpf, usu.usunome
						order by descricao";
                $gestor = pegaGestor($coaid,$_SESSION['unidade']);
                if ($retorno == true){
                	$db->monta_combo('gestor', $sql, 'N', "Selecione um gestor...", '', '', '', '400', 'S', 'u',false,null,'Gestor');
                } else {
                	$db->monta_combo('gestor', $sql, 'S', "Selecione um gestor...", '', '', '', '400', 'S', 'u',false,null,'Gestor');
                }
                                ?>  
                <div id="gestorAdd"></div>   
                </td>
            <tr>
                <td class ="SubTituloDireita" align="right">Especificação do item: </td>
                <td>
                <?php
                if($coaid){ 
					$sql = "SELECT
	                			c.coiid, 
	                			c.coidsc,
	                			c.coiitem,
	                			c.coiespecificacao, 
	                			c.coiqtde
	                		FROM 	
	                			evento.coitem AS c 
	                		WHERE 
	                			c.coiiditempai IS NULL
	                		AND
							c.coiid in ( 
									SELECT 
									coiid 
									FROM evento.coitemprocesso 
									WHERE copid = '".$_SESSION['copid']."' 
								)
							AND c.coiid NOT IN (
								SELECT DISTINCT 
									c.coiid
								FROM evento.codemandaitem AS cd 
								INNER JOIN evento.coitemprocesso AS ci ON ci.cotid = cd.cotid
								INNER JOIN evento.coitem AS c ON c.coiid = ci.coiid
								WHERE cd.coaid = {$coaid} )  
	                ";  
	                $rsComboItem = $db->carregar( $sql );
                }
                $disable = "";
		    	if($boAlterar && $_GET['boExibeCabecalho'] == 'N'){
		    		$disable = "disabled=\"disabled\"";
		    	}
		    	if ($retorno == false){
		    		echo"<select $disable name=\"coitem\" value =\"\"  onclick=\"return limparCampos();\" onchange=\"carregaDetalheItem(this.value,'fixo');\"  id=\"coitem\"  class=\"CampoEstilo\" style=\"width: 500px;\" disabled='disabled'> ";
                } else {
                	echo '<select disabled="disabled">';	
                }
                echo"<option value=''>Selecione um item de compra...</option>";

                if($boAlterar && $_GET['boExibeCabecalho'] != 'N'){
	                $numItems = 0;
	                for( $i = 0; $i<count( $rsComboItem); $i++){   
	                	 
	                		$numItems++;
		                	if( $rsComboItem[$i]['coiid'] != '' ){      
		                		$sqlProcessoItem[$i]   = "SELECT cotid FROM evento.coitemprocesso WHERE coiid = {$rsComboItem[$i]['coiid']} AND copid = {$_SESSION['copid']} ";
	                	 
	            				$cotid[$i]  = $db->pegaUm($sqlProcessoItem[$i] );            		 
			                	echo"
			                		<option value = \"{$rsComboItem[$i]['coiid']}\"  >    {$rsComboItem[$i]['coiitem']}  -  {$rsComboItem[$i]['coidsc']} </div> </option>                		
			                	";	  
	                	}else{ 
	                		continue;
	                	}
	                }       
	                if( $numItems == 0){ 
			        	echo" <option value = \"\"  > Nenhum item disponível para este Processo</option>";              
	                }
                }
                ?>  
                <div id="itemAdd"></div>   
                </td>
                <td rowspan="5" style="background-color:#fafafa; color:#404040; width:1%" valign="top" align="left">
				<?php 
				if( $_SESSION['coaid'] ){
						$docid = evtCriarDocCompra($_SESSION['coaid']);
						wf_desenhaBarraNavegacao( $docid , array( '' => '' ) );		
					}		
				?>
				</td>  
            </tr>  
         	<tr>
                <td class ="SubTituloDireita" align="right">Unidade de Medida: </td>
                <td id="unidadeMedida"> 
                </td> 
            </tr> 
            <tr>
                <td class ="SubTituloDireita" align="right">Especificação: </td>
                <td id="td_especificacao">                 	
                </td> 
            </tr> 
            <tr> 
             	<input type="hidden" name="hid_coivlrreferenciamin" id="hid_coivlrreferenciamin" value = 0>
             	<input type="hidden" name="hid_coivlrreferenciamax" id="hid_coivlrreferenciamax" value = 0>
             	<input type="hidden" name="cotid" id="cotid" value ="">
             	<input type="hidden" name="update" id="update" value = "f">              
            	<td class ="SubTituloDireita" align="right">
            		Valor: 
            	</td>
            	<td id="tipoValor">  
            	 	<b>Mínimo (R$): </b><label name="lb_coivlrreferenciamin" id="lb_coivlrreferenciamin"></label> 
            	 	<br>
            	 	<b>Máximo (R$): </b><label name="lb_coivlrreferenciamax" id="lb_coivlrreferenciamax"></label>
           		 </td>
           	</tr> 
           	<tr>
           	<td class ="subTituloDireita" >
           		Distribuição das Quantidades:
           		</td>
           		<td>
           		<div id="tb_dias">
           		<?
           		if( $coaid ){
		           		$sql = "select distinct c.coeid, c.coaid, c.coenddsc from evento.coenderecoentrega as c
								left join evento.codemandaitem as coi ON coi.coeid = c.coeid AND coi.coaid = c.coaid
								where c.coaid = {$coaid} 
								 "; //falta where da unidade para efeito de testes
		           		 $rsEndUnidade = $db->carregar( $sql );  
					}
			 
	           	 if( $rsEndUnidade ){
	           	 	$numUnidades = count( $rsEndUnidade );
	           	 }else{
	           	 	$numUnidades = 0;
	           	 } 
	           	  
	           	  ?>
           	 
           		<input type="hidden" name="numUnidades" id="numUnidades" value="<?=$numUnidades; ?>"> 
           		<table class="tabela listagem" width="60%" border='0'> 
	           		<tr>
	           			<td width="35%"><b> Unidade</b> </td>
	           			<td width="5%"><b> Quantidade</b></td> 
	           			<td width="30%"><b> Valor Mínimo</b></td>
	           			<td width="30%"><b> Valor Máximo</b></td>
	           		</tr> 
	           		<?
	           		
	           		$readonly = "";
			    	if($boAlterar && $_GET['boExibeCabecalho'] == 'N'){
			    		$readonly = "readonly";
			    	}
	           		
	           		if( $rsEndUnidade ){
		           		for( $i= 0; $i<count($rsEndUnidade); $i++){
		           			if ($retorno == 1){
		           				echo '<tr>
		           					<td>
			           					<a style="cursor: pointer;"onclick=" return direcionarEndereco( '.$rsEndUnidade[$i]['coeid'].' ); " >'.$rsEndUnidade[$i]['coenddsc'].' </a>
			           				</td>
		           					<td> 
		           						<input disabled type="input" '.$readonly.' size="8" name="coiqtde[]" id="coiqtde[]" onkeyup=\" calculaTudo();\" value="" />
			           					<input type="hidden" name="coeid['.$i.']" id="coeid['.$i.']" value="'.$rsEndUnidade[$i]['coeid'].'"> 
		           					</td>
		           					<td id="colunaValorMin['.$i.']"> 
		           					</td>
			           				<td id="colunaValorMax['.$i.']"> 
			           				</td> 
		           				    </tr>';
		           			} else {
		           				echo '<tr>
		           					<td>
			           					<a style="cursor: pointer;"onclick=" return direcionarEndereco( '.$rsEndUnidade[$i]['coeid'].' ); " >'.$rsEndUnidade[$i]['coenddsc'].' </a>
			           				</td>
		           					<td> 
		           						<input type="input" '.$readonly.' size="8" name="coiqtde[]" id="coiqtde[]" onkeyup=\" calculaTudo();\" value="" />
			           					<input type="hidden" name="coeid['.$i.']" id="coeid['.$i.']" value="'.$rsEndUnidade[$i]['coeid'].'"> 
		           					</td>
		           					<td id="colunaValorMin['.$i.']"> 
		           					</td>
			           				<td id="colunaValorMax['.$i.']"> 
			           				</td> 
		           				    </tr>';	
		           			}
		           		}
	           		}else{
	           			 echo '<tr>
		           					<td colspan ="4">
		           					<i>Não existem endereços de entrega cadastrados.</i> 
		           				    </tr>';
	           		}
					?>
	           		<tr>
	           			<td width="20%"  > 
	           			Total
	           			</td>
	           			<td id="colunaQtdTotal" width="20%"> 
	               		</td>
	               		<td id="colunaTotalValorMin" width="20%" >
	           			</td>
	           			<td id="colunaTotalValorMax">
	           			</td>
	               	</tr>
           		</table> 
          	</div>
        	</td>  
           	</tr> 
        	<input type="hidden" name="countEndereco" id="countEndereco" value="<?=$numUnidades; ?>"> 
           	<?php if( $rsEndUnidade  ) {?>
           	<tr>
            	 <td  class ="SubTituloDireita" align="right"> 
            	 </td>
            	 <td>
	               <input type="hidden" name="action" value="0" id="action">
	               <?php
	               if($boAlterar && $_GET['boExibeCabecalho'] != 'N'){
	               if($retorno <> 1){
	               	?>
	               <input type="button" name="btnGravar" value="Gravar" id="btnGravar" onclick="validaForm('grava');">
	               <?php 
	               }} 
	               ?>  
            	 </td> 
            </tr>
            <?} ?>	 
            <table border="0" cellspacing="0" cellpadding="3" align="center" class="Tabela">
   			<?
   			$dados = "";
   			if($coaid){ 
   				if ($retorno == true){
	            	$sql = "
					SELECT DISTINCT 
							'<center>-</center>'  as acao,                       
	                     	c.coidsc,
	                     	ci.cotid,
	            			c.coiqtde,
	            			c.coivlrreferenciamin,
	            			c.coivlrreferenciamax 
					FROM evento.codemandaitem AS cd 
					INNER JOIN evento.coitemprocesso AS ci ON ci.cotid = cd.cotid
					INNER JOIN evento.coitem AS c ON c.coiid = ci.coiid
					 where cd.coaid = {$coaid }
					"; 
   				} else {
   					$sql = "
					SELECT DISTINCT 
							'<center><a href=\"javascript: carregaDetalheItemCadastrado('|| cd.cotid ||', ''cadastrado'', \''|| c.coidsc ||'\');\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Alterar\"> <a href=\"#\" onclick=\"javascript:excluirItemProcesso('|| cd.cotid ||', ''cadastrado'');\"><img src=\"/imagens/excluir.gif\" border=0 title=\"Excluir\"></a></center>'  as acao,                       
	                     	c.coidsc,
	                     	ci.cotid,
	            			c.coiqtde,
	            			c.coivlrreferenciamin,
	            			c.coivlrreferenciamax 
					FROM evento.codemandaitem AS cd 
					INNER JOIN evento.coitemprocesso AS ci ON ci.cotid = cd.cotid
					INNER JOIN evento.coitem AS c ON c.coiid = ci.coiid
					 where cd.coaid = {$coaid }
					";	
   				}
	            $cabecalho = array( "Ação", "Descrição", "Quantidade","Valor de Referência Mínimo", "Valor de Referência Máximo");
	           // $db->monta_lista( $sql, $cabecalho, 25, 10, 'N', '', '' );
	      
	            //não descomentar trecho abaixo por enquanto.
	            $dados = $db->carregar( $sql );
   			}
 	 
    if( $dados ){ 
    	?>
    	<table class="listagem" width="95%" align="center" border="0" cellpadding="2" cellspacing="0">
		    <input name="numero" value="" type="hidden">
		    <input name="ordemlista" value="" type="hidden">
		    <input name="ordemlistadir" value="" type="hidden">
		    <thead>
		    <tr>
		    <?php
		    	if($boAlterar && $_GET['boExibeCabecalho'] != 'N'){
			?>
		    <td class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" valign="top" bgcolor="">
		    <strong>Ação</strong>
		    </td>
		    <?php
		    	}
			?> 
		    <td class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"  valign="top" bgcolor="">
		    <strong>Descrição</strong>
		    </td>
		    <td class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"valign="top" >
		    <strong>Quantidade</strong>
		    </td>
		    <td class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"valign="top" >
		    <strong>Valor de Referência Mínimo</strong>
		    </td>
		    <td class="title" style="border-left: 1px solid rgb(255, 255, 255); border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192);" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"valign="top" >
		    <strong>Valor de Referência Máximo</strong>
		    </td> 
		    </tr> 
		    </thead>
		   <?
		   for( $i = 0; $i < count( $dados ); $i++)
		   {
		   if(($i % 2) == 1) {
		   	$fundo="#F7F7F7";
		   }else {
		   		$fundo="#FFFFFF";	
		   }
			?> 
			    <tbody>
			    <tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?=$fundo ?>';" bgcolor="<?=$fundo ?>">
				    <?php
				    	if($boAlterar && $_GET['boExibeCabecalho'] != 'N'){
					?>
				    <td title="Ação">
					    <?php 
					    	$JsFunction = "javascript:carregaDetalheItemCadastrado({$dados[$i]['cotid']}, 'cadastrado', '{$dados[$i]['coidsc']}');";
					    ?>
					    <center>
					    <?php if ($retorno == false){ ?>
					      <a href="#" onclick="<?=$JsFunction?>">
						     <img src="/imagens/alterar.gif" title="Alterar" border="0"></a> 
						  <a href="#" onclick="javascript:excluirItemProcesso(<?=$dados[$i]['cotid']; ?>);">
						  <img src="/imagens/excluir.gif" title="Excluir" border="0"></a>
					    <?php
					    } else { ?>-<?php }} ?>
					    </center>
				    </td>
				    <td >
				    	<?php
							echo $dados[$i]['coidsc'];
				    	?> 
				    </td>
				    <td style="color: rgb(0, 102, 204);"  title="Quantidade" align="right">
				    	<?php
				    		$sqlSomaQtd  = "select sum(cdiqtde) from evento.codemandaitem where coaid = {$_SESSION['coaid']} and cotid = {$dados[$i]['cotid']}";
				    		$somaQtd   = $db->pegaUm( $sqlSomaQtd );
				    	
							echo $somaQtd;
				    	?>  
				    </td> 
				    <td title="Valor de Referência Mínimo" style="color: rgb(0, 102, 204);" align="right">
						<?php
							 
							echo number_format(( $somaQtd * $dados[$i]['coivlrreferenciamin'] ),2,",",".");
				    	?>  	
				    </td>
				    <td title="Valor de Referência Máximo" align="center" style="color: rgb(0, 102, 204);" title="Valor Parcial/Total">
				    	<?php
							echo number_format(( $somaQtd * $dados[$i]['coivlrreferenciamax'] ),2,",",".");
				    	?>  
				    </td>  
				</tr> 
			<?
	    	}
	    ?>
	    <tr>
		    <td  onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';" valign="top" align="" bgcolor="">
		    <strong><B><center>TOTAL: <?=count($dados) ?></center></B></strong>
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
		    <strong></strong>
		    </td> 
		    </tr>  
		<?
    }
    else
    {
    	?>
    	<table class="listagem" width="95%" align="center" border="0" cellpadding="2" cellspacing="0">
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
 
 
function validaForm(tipo){   
	var action = document.getElementById('action');
	var coitem = document.getElementById('coitem');
	//var numUnidades = document.getElementById('numUnidades');
	var update = document.getElementById("update");
	var coiqtde;
	<?
	 if( $numUnidades != '' )
	 {
	 	 ?>	
		 var numUnidades 	  = Number(<?= $numUnidades; ?>);
		 <?
	 }	  
	 ?>	
	if( update.value != 't' ){ 
		if( coitem.value == '' ){
			alert("É necessário selecionar algum item para realizar esta operação");
			return false;
		} 
	}
	var soma = 0; 
	for( var i= 0; i< numUnidades; i++ ){
	 	 coiqtde = Number( document.getElementById('coiqtde['+i+']').value);
	 	 soma = soma + coiqtde;
	 	 /*if( ( coiqtde == 0 ) || ( coiqtde == '' ) ){
	 	 	alert("É necessário informar uma quantidade maior que 0(zero)");
	 	 	return false;
	 	 }*/
	}
	if( ( soma == 0 ) ){
 		alert("É necessário informar uma quantidade maior que 0(zero)");
 		return false;
 	}
 	
 	if (document.formulario.gestor.value == 0) {
 		alert("É necessário informar um gestor para a demanda.");
 		document.formulario.gestor.focus();
 		return false;
 	}	
	 	 
	 	 
	action.value = 'grava';
	document.formulario.submit();	 
}
   
function carregaDetalheItem( id , tipo)
{   
	if( id == '' ){ 
		return false;
	} 
	top.update = 'false';  
	var td_especidicacao = document.getElementById("td_especificacao");
	var hid_cotid 			 		 = document.getElementById('cotid');  
 	var coitem 	 					 = document.getElementById('coitem');
 	var hid_coivlrreferenciamin 	 = document.getElementById('hid_coivlrreferenciamin'); 	
 	var hid_coivlrreferenciamax 	 = document.getElementById('hid_coivlrreferenciamax');
 	var lb_coivlrreferenciamin 		 = document.getElementById('lb_coivlrreferenciamin'); 
 	var lb_coivlrreferenciamax 		 = document.getElementById('lb_coivlrreferenciamax');
 	var hid_update		 = document.getElementById('update'); 
 	var hid_descricao 	 = document.getElementById('hid_descricao'); 	 
 	var tb_dias 	  	 = document.getElementById('tb_dias');
 	//var numUnidades 	 = document.getElementById('numUnidades'); 
 	<?
	 if( $numUnidades != '' )
	 {
	 	 ?>	
		 var numUnidades 	  = Number(<?= $numUnidades; ?>);
		 <?
	 }	  
	 ?> 	
 	var unidadeMedida = document.getElementById('unidadeMedida');
 	var conteudo_link;
 	var req = new Ajax.Request('evento.php?modulo=principal/cadCompraInfra&acao=A&tipo='+tipo, {
							        method:     'post',
							        parameters: '&ajaxitem=' + id,							         
							        onComplete: function (res)
							        {   
							         
										arrResponse = res.responseText.split("_"); 
										var tdDias = '';
										var coiid			  	= arrResponse[0];
									    var coidsc		 	  	= arrResponse[1];
									 	var umeid 	 	  		= arrResponse[2];
										var umedescricao   		= arrResponse[3];
										var coivlrreferenciamin = arrResponse[4];
										var coivlrreferenciamax = arrResponse[5];
										var coiespecificacao 	= arrResponse[6];
										var cotid 				= arrResponse[7];
										
										var arrEndereco 		= arrResponse[8];
										var arrCoeid 			= arrResponse[9];
										 
										if( coiid != '' ){ 
								 			conteudo_link = "<a style=\"cursor: pointer;\" onclick=\"return abirPopupEspecificao('"+coiid+"');\"> <img align=\"absmiddle\" src=\"/imagens/consultar.gif\" title=\"Visualizar Detalhes da Especificação\"> Visualizar Especificações do item </a>"; 
										}else{
											conteudo_link = "<i>item sem especificação</i>"; 
										}
										 
										td_especificacao.innerHTML = conteudo_link;
							 			unidadeMedida.innerHTML = '<b>'+umedescricao+'</b>';
							 			  
										lb_coivlrreferenciamin.innerHTML 		= '<input type="hidden" name="cotid" id="cotid" value = '+cotid+'> <input type="hidden" name="coiid" id="coiid" value = '+coiid+'> &nbsp;&nbsp;&nbsp; '+float2moeda(coivlrreferenciamin)+' '; 
										lb_coivlrreferenciamax.innerHTML 		= ' &nbsp;&nbsp;&nbsp; '+float2moeda(coivlrreferenciamax)+' '; 
											
											arrEndereco = arrEndereco.split(","); 
											arrCoeid 	= arrCoeid.split(","); 
											tdDias  = "<table class=\"tabela listagem\" width=\"60%\"> "; 
											tdDias += "<tr><td width=\"35%\"><b> Unidade </b> </td><td width=\"5%\"> <b>Quantidade</b></td> <td width=\"30%\"> <b>Valor Mínimo</b></td><td width=\"30%\"> <b>Valor Máximo</td></b></tr>";  
           									for( var i = 0; i < numUnidades; i++){   
								           		tdDias += "<tr> <td width=\"20%\" ><a style=\"cursor: pointer;\"onclick=\" return direcionarEndereco( "+arrCoeid[i]+" ); \" > "+arrEndereco[i]+"</a> </td>";
								           		tdDias += " <td width=\"20%\"> <input type=\"text\" size=\"4\" class=\"normal\" name=\"coiqtde["+i+"]\" id=\"coiqtde["+i+"]\" value=\"\" onkeyup=\"this.value=mascaraglobal('###############',this.value); calculaTudo();\">  <input type=\"hidden\" name=\"coeid["+i+"]\" id=\"coeid["+i+"]\" value="+arrCoeid[i]+"></td>";
												tdDias += "<td id=\"colunaValorMin["+i+"]\" width=\"20%\" > </td>"; 
								           		tdDias += "  <td id=\"colunaValorMax["+i+"]\" width=\"20%\" > </td></tr>";
							               	} 
								            tdDias +="	<tr> <td width=\"20%\"   > Total</b> </td><td id=\"colunaQtdTotal\" ></td><td id=\"colunaTotalValorMin\" width=\"20%\"></td><td id=\"colunaTotalValorMax\" width=\"20%\" ></td></tr>";
							            	tdDias += "</table>"; 
									 	   	tb_dias.innerHTML	= tdDias; 	 
										
										hid_update.value = 'f';  
									 	 
										hid_coivlrreferenciamin.value 	= coivlrreferenciamin;
										hid_coivlrreferenciamax.value 	= coivlrreferenciamax;  
										hid_cotid.value = cotid; 
										coitem.value = coiid;
										 
										calculaTudo(); 
										
							        } 
							  });
}
 
function carregaDetalheItemCadastrado( id , tipo, descricao )
{   
	
	if( id == '' ){ 
		return false;
	}  
 
	var conteudo_link ='';
	var td_especidicacao = '';
	var td_especidicacao = document.getElementById("td_especificacao");
	var hid_cotid 		 = document.getElementById('cotid'); 
 	var coitem 	 		 = document.getElementById('coitem');
 	 
 	var hid_coivlrreferenciamin 	 = document.getElementById('hid_coivlrreferenciamin'); 	
 	var hid_coivlrreferenciamax 	 = document.getElementById('hid_coivlrreferenciamax');
 	var lb_coivlrreferenciamin 		 = document.getElementById('lb_coivlrreferenciamin'); 
 	var lb_coivlrreferenciamax 		 = document.getElementById('lb_coivlrreferenciamax');
 	var hid_update		 = document.getElementById('update'); 
 	var hid_descricao 	 = document.getElementById('hid_descricao'); 	 
 	//var numUnidades 	 = document.getElementById('numUnidades'); 
 	<?
	 if( $numUnidades != '' )
	 {
	 	 ?>	
		 var numUnidades 	  = Number(<?= $numUnidades; ?>);
		 <?
	 }	  
	 ?>	 
 	var select_coitem    = document.getElementById('coitem');
 	
 	var unidadeMedida = document.getElementById('unidadeMedida');
 	var tb_dias 	  = document.getElementById('tb_dias');
 	var req = new Ajax.Request('evento.php?modulo=principal/cadCompraInfra&acao=A&tipo='+tipo, {
							        method:     'post',
							        parameters: '&ajaxitemCadastrado=' + id,							         
							        onComplete: function (res)
							        {  
							        	
										arrResponse = res.responseText.split("_"); 
										//alert( res.responseText );
										var coiid			  	= arrResponse[0];
									    var coidsc		 	  	= arrResponse[1];
									 	var umeid 	 	  		= arrResponse[2];
										var umedescricao   		= arrResponse[3];
										var coivlrreferenciamin = arrResponse[4];
										var coivlrreferenciamax = arrResponse[5];
										var coiespecificacao 	= arrResponse[6]; 
										var arrDias				= arrResponse[7];
										var arrEndereco			= arrResponse[8];
										var arrCoeid			= arrResponse[9]; 
										var cotid 				= arrResponse[10];
										
										var tdDias = '';
											arrDias 	= arrDias.split(",");  
											arrEndereco = arrEndereco.split(","); 
											arrCoeid 	= arrCoeid.split(","); 
											  
											tdDias  = "<table class=\"tabela listagem\" width=\"60%\"> "; 
											tdDias += "<tr><td width=\"35%\"><b> Unidade </b> </td><td width=\"5%\"> <b>Quantidade</b></td> <td width=\"30%\"> <b>Valor Mínimo</b></td><td width=\"30%\"> <b>Valor Máximo</td></b></tr>";  
           									 
							           		for( var i = 0; i < numUnidades; i++)
							           		{   			 		
								           		tdDias += "<tr> <td width=\"20%\" ><a style=\"cursor: pointer;\"onclick=\" return direcionarEndereco( "+arrCoeid[i]+" ); \" > "+arrEndereco[i]+"</a> </td>";
								           		tdDias +=" <td  width=\"20%\"> <input type=\"text\" size=\"8\" class=\"normal\"name=\"coiqtde["+i+"]\" id=\"coiqtde["+i+"]\" value='"+arrDias[i]+"' onkeyup=\"this.value=mascaraglobal('###############',this.value); calculaTudo();\">  <input type=\"hidden\" name=\"coeid["+i+"]\" id=\"coeid["+i+"]\" value="+arrCoeid[i]+"> </td>";
												tdDias +="<td id=\"colunaValorMin["+i+"]\" width=\"20%\" > </td>"; 
								           		tdDias += "  <td id=\"colunaValorMax["+i+"]\" width=\"20%\" > </td></tr>";
							               	}
							               	
								            tdDias +="	<tr> <td width=\"20%\" > Total</b> </td><td id=\"colunaQtdTotal\"  ></td><td id=\"colunaTotalValorMin\" width=\"20%\"></td><td id=\"colunaTotalValorMax\" width=\"20%\" ></td></tr>";
							            	tdDias += "</table>";
							            	tb_dias.innerHTML	= tdDias; 
							            	
							            if( coiid != '' ){  
								 			td_especificacao.innerHTML = "<a style=\"cursor: pointer;\" onclick=\"return abirPopupEspecificao('"+coiid+"');\"> <img align=\"absmiddle\" src=\"/imagens/consultar.gif\" title=\"Visualizar Detalhes da Especificação\"> Visualizar Especificações do item </a>"; 
										}else{
											td_especificacao.innerHTML = "<i>item sem especificação"; 
										} 
									  
							 			unidadeMedida.innerHTML = '<b>'+umedescricao+'</b>';
							 			 
										lb_coivlrreferenciamin.innerHTML 		= '<input type="hidden" name="cotid" id="cotid" value = '+cotid+'> <input type="hidden" name="coiid" id="coiid" value = '+coiid+'> &nbsp;&nbsp;&nbsp; '+float2moeda(coivlrreferenciamin)+' '; 
										lb_coivlrreferenciamax.innerHTML 		= ' &nbsp;&nbsp;&nbsp; '+float2moeda(coivlrreferenciamax)+' '; 
										
										hid_coivlrreferenciamin.value 	= coivlrreferenciamin;
										hid_coivlrreferenciamax.value 	= coivlrreferenciamax; 
										  
										hid_update.value = 't';  
										select_coitem.options[0].text = '...';
										select_coitem.options[0].text = descricao;
										select_coitem.disabled = 1;
										calculaTudo();
										 
							        } 
							  });
}

function abirPopupEspecificao( id ){
	 
	 return window.open('evento.php?modulo=principal/popupCompraEspecificacao&acao=A&link=S&id='+id,
                  'popupCompraEspecificacao',
                  'height=600,width=800,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
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


function excluirItemProcesso(valor){ 
	if( valor != ''){ 
		return location.href='evento.php?modulo=principal/cadCompraInfra&acao=A&excluir='+valor; 
	} 
}	

function calculaTudo()
 {  
 	<?
	 if( $numUnidades != '' )
	 {
	 	 ?>	
		 var numUnidades 	  = Number(<?= $numUnidades; ?>);
		 <?
	 }	  
	 ?>	
	// var numUnidades  = document.getElementById('numUnidades'); 
	 
	 var coiqtde = 0;  
	 var hid_coivlrreferenciamin 	 = document.getElementById('hid_coivlrreferenciamin'); 	
	 var hid_coivlrreferenciamax 	 = document.getElementById('hid_coivlrreferenciamax'); 	
 	 var valorMin =0;
 	 var valorMax =0;
 		
 	  
	 for( var i= 0; i< numUnidades; i++ ){
	 
	 	 var coiqtde = document.getElementById('coiqtde['+i+']');
	 	 valorMin = Number( hid_coivlrreferenciamin.value * Number(coiqtde.value) );
	 	 valorMax = Number( hid_coivlrreferenciamax.value * Number(coiqtde.value) );
 
	  	 if( isNaN( valorMin ) ){ 
	  	 	 colunaValorMin      			= document.getElementById('colunaValorMin['+i+']'); 
		 	 colunaValorMin.innerHTML 		= 0;    	 	 
		 }
		 else{
		 	 colunaValorMin 				= document.getElementById('colunaValorMin['+i+']'); 
		 	 colunaValorMin.innerHTML 		= float2moeda(valorMin);   
		 	
		 } 
		 if( isNaN( valorMax ) ) { 
	  	 	 colunaValorMax      			= document.getElementById('colunaValorMax['+i+']'); 
		 	 colunaValorMax.innerHTML 		= 0;  		 	 
		 }
		 else{
		 	 colunaValorMax 				= document.getElementById('colunaValorMax['+i+']'); 
		 	 colunaValorMax.innerHTML 		= float2moeda(valorMax);   
		 }  
	 }     
	calculaTotal( numUnidades );  
}

function calculaTotal(numUnidades ){ 
	 var valorTotalMin = 0;
 	 var valorTotalMax = 0;
 	 var valorTotal	   = 0;
 	 var colunaTotalValorMin = document.getElementById('colunaTotalValorMin'); 	
 	 var colunaTotalValorMax = document.getElementById('colunaTotalValorMax');
 	 var colunaQtdTotal      = document.getElementById('colunaQtdTotal'); 	
 	 var valorParcialMin = 0;
 	 var valorParcialMax = 0;
 	 var coeqtde = 0;
	 for( var i= 0; i< numUnidades; i++ ){  
	 	 
	 	 coiqtde  = document.getElementById('coiqtde['+i+']');  
	 	 valorTotal += Number( coiqtde.value );
	 	  
	  	 colunaValorMin  = document.getElementById('colunaValorMin['+i+']');  
	  	 valorParcialMin = colunaValorMin.innerHTML; 
	  	 valorParcialMin = valorParcialMin.replace(".","");
		 valorParcialMin = valorParcialMin.replace(",",".");
		 valorTotalMin  += Number( valorParcialMin );
		 
		 colunaValorMax  = document.getElementById('colunaValorMax['+i+']'); 
		 valorParcialMax = colunaValorMax.innerHTML; 
	  	 valorParcialMax = valorParcialMax.replace(".","");
		 valorParcialMax = valorParcialMax.replace(",",".");		 
		 valorTotalMax  += Number( valorParcialMax );
	 }   
	 colunaQtdTotal.innerHTML 	   = valorTotal;
	 colunaTotalValorMin.innerHTML = float2moeda(valorTotalMin);		
	 colunaTotalValorMax.innerHTML = float2moeda(valorTotalMax);		
}


function limparCampos()
{ 
	var td_especificacao 			 = document.getElementById("td_especificacao"); 
	var numCamposForm 				 = document.getElementById("formulario").length; 
	//var numCampos					 = document.getElementById('numUnidades');
	<?
	 if( $numUnidades != '' )
	 {
	 	 ?>	
		 var numCampos 	  = Number(<?= $numUnidades; ?>);
		 <?
	 }	  
	 ?>	
	var cotid 			 			 = document.getElementById('cotid');  
 	var coitem 	 					 = document.getElementById('coiid');
 	var hid_coivlrreferenciamin 	 = document.getElementById('hid_coivlrreferenciamin'); 	
 	var hid_coivlrreferenciamax 	 = document.getElementById('hid_coivlrreferenciamax');
 	var lb_coivlrreferenciamin 		 = document.getElementById('lb_coivlrreferenciamin'); 
 	var lb_coivlrreferenciamax 		 = document.getElementById('lb_coivlrreferenciamax'); 
 	var tb_dias 	  				 = document.getElementById('tb_dias');
 	var numUnidades 	 			 = document.getElementById('numUnidades'); 
 	var unidadeMedida 				 = document.getElementById('unidadeMedida');
 	var colunaTotalValorMin 		 = document.getElementById('colunaTotalValorMin');
 	var colunaTotalValorMax 		 = document.getElementById('colunaTotalValorMax');
	var select_coitem				 = document.getElementById('coitem');
	for( var j = 0; j< numCampos; j++ ){ 		
 		document.getElementById('colunaValorMin['+j+']').innerHTML = '';
 		document.getElementById('colunaValorMax['+j+']').innerHTML = '';
 	}
 	colunaTotalValorMin.innerHTML 	  = '';  
	colunaTotalValorMax.innerHTML 	  = '';  
 	var idForm;
 	var idFormSubstr;
	for( var i= 0; i< numCamposForm; i++){
		idForm 			= document.formulario[i].id;
		idFormSubstr 	= idForm.substr( 0, 5 );
 
		if( (document.formulario[i].id != 'btnGravar') 
		
		&& ( idFormSubstr != 'coeid')  
		&& ( document.formulario[i].id != 'btnLimpar')
		&& ( document.formulario[i].id != 'numUnidades') ){
		//	alert('vai limpar: '+ document.formulario[i].id );
			document.formulario[i].value = ''; 
		} 
	}	 
	lb_coivlrreferenciamin.innerHTML  = '';
	lb_coivlrreferenciamax.innerHTML  = '';
	
	td_especificacao.innerHTML 		  = '';
	unidadeMedida.innerHTML	  		  = '';   
	select_coitem.disabled = 0;
}

function verificaEnderecoEntrega(){
	 var numUnidades 	  = document.getElementById('numUnidades');
	 var select_coitem    = document.getElementById('coitem');
	 if( numUnidades.value == 0 ){
	  	var select_coitem_desabled    = document.getElementById('coitem'); 
	 	select_coitem_desabled.disabled = 1;
	 }else{
	 	var select_coitem_enabled    = document.getElementById('coitem');
	 	select_coitem_enabled.disabled = 0;
	 }
}
function direcionarEndereco(id){
 
 	if( id ){
 	 return window.open('evento.php?modulo=principal/popupEnderecoCompra&acao=A&coeid='+id,
                  'popupEnderecoCompra',
                  'height=420,width=800,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes'); 
	}
}
verificaEnderecoEntrega(); 
 </script>