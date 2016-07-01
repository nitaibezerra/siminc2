<head>
   <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script src="../includes/prototype.js"></script>
    <script src="../includes/entidades.js"></script>
    <script src="../includes/calendario.js"></script>
    
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/> 
</head> 
<?php  
if( $_SESSION['evento']['eveid'] != '')
{ 
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
				ev.evepublicoestimado,  
				ev.evequantidadedias,
				ev.muncod, 				 
				ev.estuf, 				 
				ev.sevid,
				ev.eveqtdpassagemaerea,
				u.ungdsc,
				us.usunome,
				ev.docid,
				ev.eveurgente,
				ev.rcoid,
				ev.endid,
				to_char(ev.evedatainclusao::date,'DD/MM/YYYY') AS evedatainclusao		 
			FROM 
				evento.evento AS ev
			LEFT JOIN evento.tipoevento AS te 
				ON te.tpeid = ev.tpeid
			LEFT JOIN 
				public.unidadegestora AS u ON ev.ungcod = u.ungcod
			LEFT JOIN seguranca.usuario AS us ON us.usucpf = ev.usucpf 
			WHERE
				ev.eveid = '".$_SESSION['evento']['eveid']."'";
	
	$rsDadosEvento = $db->carregar( $sql );
	 
}
if( $_SESSION['evento']['eveid'] != ''){
	$docid = evtCriarDoc( $_SESSION['evento']['eveid'] );
}
 
if( $_POST['ajaxunsetsession'] == '1'){
	$_SESSION['evento']['eveid'] = ''; 
	die();
}

$sql   = "SELECT * FROM evento.avaliacaoevento WHERE eveid = ".$_SESSION['evento']['eveid'];
$dados = $db->carregar( $sql );
if( $dados ){
	
}

function verificaMarcacao($qavid, $eavid){
	global $db;
	$sql = "SELECT eavid FROM evento.avaliacaoevento WHERE eveid = ".$_SESSION['evento']['eveid']." AND qavid = $qavid AND eavid = $eavid";
	$tem = $db->pegaUm( $sql );
	if( $tem ){
		echo "checked=checked";		
	}else{
		echo "";
	}
}

if( $_POST['action'] == 'salva' ){	
 
	$arEscalas = array_values( $_POST['escala'] ); 
	$sqlD  = "DELETE FROM evento.avaliacaoevento WHERE eveid = ".$_SESSION['evento']['eveid'];
	$sqlD2 = "DELETE FROM evento.avaliacaosubjetivaevento WHERE eveid = ".$_SESSION['evento']['eveid'];
	$db->executar( $sqlD );
	$db->executar( $sqlD2 );
	for( $i=0; $i<count( $arEscalas ); $i++){
		$arr[$i] = explode("_", $arEscalas[$i] );
		$qavid 	 = $arr[$i][0];
		$eavid 	 = $arr[$i][1];
		$sql 	 = "INSERT INTO evento.avaliacaoevento (eveid, qavid, eavid ) VALUES ( ".$_SESSION['evento']['eveid'].",".$qavid.",".$eavid." )";
		$db->executar( $sql );
	}
	for( $k = 0; $k < count( $_POST['rasresposta']); $k++){
		$sql = "INSERT INTO evento.respostaavaliacaosubjetivaeve ( rasresposta ) VALUES ('".$_POST['rasresposta'][$k+1]."') RETURNING rasid";
		$rasid = $db->pegaUm( $sql );
		$sql = "INSERT INTO evento.avaliacaosubjetivaevento (eveid, qusid, rasid ) VALUES ( ".$_SESSION['evento']['eveid'].", ".$_POST['hid_qusid'][$k].", ".$rasid." )";
		 $db->executar( $sql );
	} 
	$db->commit();	 
	echo("<script>alert('Operação realizada com sucesso.')\n</script>");
    echo("<script>window.location.href = 'evento.php?modulo=principal/avaliacaoEvento&acao=A';</script>");
	exit();		
}

$docid = evtCriarDoc($_SESSION['evento']['eveid']);
$esdid = verificaEstadoDocumento($docid);

$ativo = 'N';
if($esdid == AGUARDANDO_EXECUCAO_EVENTO_WF){
	$ativo = 'S';
}

$titulo_modulo = "Eventos";
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
monta_titulo( $titulo_modulo, 'Avaliação de Serviços.' );
echo'<br>'; 
?>
<br>
 <? 
$res = array( 
 				 array ( "descricao" => "Lista",
						    "id" 		=> "5",
						    "link" 		=> "/evento/evento.php?modulo=inicio&acao=C&submod=evento"
				  		  ),
				  array ( "descricao" => "Informações Básicas",
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
*
if($_SESSION['evento']['eveid']){
	array_push($res,	
				  	array ("descricao" => "Estrutura Orçamentária",
						    "id"		=> "6",
						    "link"		=> "/evento/evento.php?modulo=principal/cadEstruturaOrcamentaria&acao=A"
				  		   )
					  );	
}*/

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

echo montarAbasArray($res, $_REQUEST['org'] ? false : "/evento/evento.php?modulo=principal/avaliacaoEvento&acao=A");

headEvento($rsDadosEvento[0]['evetitulo'],$rsDadosEvento[0]['everespnome'], $rsDadosEvento[0]['ungdsc'], $rsDadosEvento[0]['ungcod'], $rsDadosEvento[0]['eveurgente'], $rsDadosEvento[0]['evedatainclusao'], false);
 echo'<br>';
 
 
 ?> 
 <body>
 <table   bgcolor="#f5f5f5" width="300px" align="center" border="0">
 <tr>
 <?php
				$sql = "select * from evento.escalaavaliacao order by eavid";
				$rsEscala = $db->carregar( $sql );
				for( $k = 0; $k <count( $rsEscala ); $k++ ){
                		?>
                			<td align="center">
                			  <?=$rsEscala[$k]['eavid'] ?>
                			  <hr>
                			  <?=$rsEscala[$k]['eavdescricao'] ?>
                			</td>
             	<?php 
				}?>
 </tr>
 </table>
 <br>
    <form name = "formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
    <?php
	$sql = "select aquid, aqudescricao from evento.assuntoquestao order by aquid ";
	$rsAssunto = $db->carregar( $sql );
	for( $i = 0; $i< count( $rsAssunto); $i++){
    ?>
         	<tr>
                <td align="left"colspan="2">
					<b><?php echo $rsAssunto[$i]['aqudescricao']; ?></b>
                </td> 
            </tr>
            <?php 
			$sql = "select q.qavid, q.qevdescricao, tq.tqadescricao 
					from evento.questaoavaliacao as q 
					inner join evento.tipoquestaoavaliacao as tq on q.tqaid = tq.tqaid 
					where q.qevstatus = 'A' 
					and q.aquid = '".$rsAssunto[$i]['aquid']."'";
            $rsQestaoAvaliacao = $db->carregar( $sql );
            for( $j = 0; $j < count( $rsQestaoAvaliacao ); $j++ ){
			?>
            <tr>
                <td  width="80%" style="background: rgb(238, 238, 238);">
                	<?php echo '<b>'.($j+1).'. '.$rsQestaoAvaliacao[$j]['tqadescricao'].' - </b>'.$rsQestaoAvaliacao[$j]['qevdescricao'] ?>
                </td>
                <td width="20%"> 
                	<table class="tabela" border="0">
                		<tr>
                		<?php
						$sql = "select * from evento.escalaavaliacao order by eavid";
						$rsEscala = $db->carregar( $sql );
						for( $k = 0; $k <count( $rsEscala ); $k++ ){
                		?>
                			<td>
                			<input type="radio" 
                				   <?= verificaMarcacao($rsQestaoAvaliacao[$j]['qavid'],$rsEscala[$k]['eavid']); ?>
                				   name="escala[]_<?=$rsQestaoAvaliacao[$j]['qavid']?>"
                				   id  ="escala[<?=$rsQestaoAvaliacao[$j]['qavid']?>][<?=$rsEscala[$k]['eavid']?>]"
                				   value="<?=$rsQestaoAvaliacao[$j]['qavid']?>_<?=$rsEscala[$k]['eavid']?>" 
                				   <?php echo $ativo == 'N' ? 'disabled="disabled"' : '' ?> > <?=$rsEscala[$k]['eavid'] ?>
                			</td>
                			
                		<?php 
						}?>
                		</tr>
                	</table>
                </td> 
            </tr>
            <?php 	
		    } 
	}
	?>
            
         <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" border="0">
        
         <?php
		 $sqlQSub = "select * from evento.questaosubjetivaevento";
		 $rsQSub = $db->carregar( $sqlQSub );
		 for( $s = 0; $s <count( $rsQSub ); $s++ ){
		 ?>
		 <tr>
			 <td class="SubTituloCentro">
	             <?php echo $rsQSub[$s]['qusdescricao'];?>   			 
			 </td>
			 <td>
			 <?php
			 $sqlResp  = "SELECT r.rasresposta FROM evento.avaliacaosubjetivaevento as a
			 				INNER JOIN evento.respostaavaliacaosubjetivaeve as r ON a.rasid = r.rasid
			 				WHERE a.eveid = ".$_SESSION['evento']['eveid']." AND a.qusid = {$rsQSub[$s]['qusid']}";
			 
			 
			 $rasresposta[$rsQSub[$s]['qusid']] = $db->pegaUm( $sqlResp );
 
			 ?>
	             <?=campo_textarea('rasresposta['.$rsQSub[$s]['qusid'].']','N',$ativo,'',100,3,'', '', '', '', '', '', $rasresposta[$rsQSub[$s]['qusid']]);?>	 
	             <input type="hidden" name="hid_qusid[]" id="hid_qusid[]" value="<?=$rsQSub[$s]['qusid'];?>">
			 </td>
		 </tr>
		<?php 
		}?>
         
         <tr>
            	<td colspan="2" align="center"> 
            	   <input type="hidden" name="action" value="0" id="action"> 
            	   <input type="hidden" name="numQuestoes" value="<?=$j ?>" id="numQuestoes"> 
            	   <input type="hidden" name="numEscala" value="<?=$k ?>" id="numEscala"> 
            	   <input type="button" name="btnGravar" value="Gravar" id="btnGravar" onclick="validaForm();" <?php echo $ativo == 'N' ? 'disabled="disabled"' : '' ?>> 
            	  </td> 
            </tr>	 
         
         </table>
         </form>  
         
   </body>       
   
<script type="text/javascript">
function validaForm(){

	var chk = validaRadios();
	if(chk){	
		var action = document.getElementById('action');
		action.value = 'salva';
		document.formulario.submit();
	}
	return false;
 
}

function validaRadios(){
	
	var numQuestoes = document.getElementById('numQuestoes').value;
	var numEscala   = document.getElementById('numEscala').value; 
 	var unchecked_total   = Number(0);
	for( var i = 1; i<= numQuestoes; i++ ){
		
		var unchecked_parcial = Number(0); 
		for( var k = 1; k<= numEscala; k++ ){ 
		
			var campo = document.getElementById('escala['+i+']['+k+']');
			if( campo.checked == false ){ 
				unchecked_parcial++;
				if( unchecked_parcial > 4 ){
					unchecked_total++;
				}				
			}
		}		
	} 
	if( unchecked_total > 0 ){
		alert("É obrigatória a marcação de todos os campos"); 
		return false;
	}
	
	return true;
}
 </script>
 
 
<?php 
//verifica permissao edição
/*
if($_SESSION['evento']['eveid']){
	$sql = "select d.esdid 
				from evento.evento e
				inner join workflow.documento as d on d.docid = e.docid
				where e.eveid = ".$_SESSION['evento']['eveid'];
	$esdid = $db->pegaUm($sql);
}	
if($esdid != EMISSAO_NF_WF){
	echo '<script>
			var obj = document.getElementsByTagName("input");
			var total = document.getElementsByTagName("input").length;
	
			for(i=0; i<total; i++){
				obj[i].disabled = true;
			}
		   </script>';
}
*/
?>