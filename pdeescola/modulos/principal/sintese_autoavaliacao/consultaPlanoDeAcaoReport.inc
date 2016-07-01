<?php
# Verfica se o pdeid e intid estão na sessão
pde_verificaSessao();
 
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

$pdeid 	= $_SESSION["pdeid"];
$entid  = $_SESSION["entid"];
$pseid = $_REQUEST['pseid'];

if (!$pdeid){
	die('<script>
			alert(\'Falta parametros!\nTente novamente.\');
			history.go(-1);
		 </script>');
}
//query que carrega os dados do subcabecalho [objetivo...]

if( $_POST['saveId'] != '' ) {	
	header('content-type: text/html; charset=ISO-8859-1'); 
		
	if( $_POST['tipo'] == 'programa'){
		$field  = "tprid";
		$field2 = "tprdescricao";
		$table  = "tipoprograma";
	}
	else {
		$field  = "tcaid";
		$field2 = "tcadescricao";
		$table  = "tipocategoria";
	}	
	$sql = "
		   UPDATE pdeescola.detalheplanoacao 
		   SET $field  = {$_POST['saveId']}
		   WHERE dpaid = {$_POST['dpaid']} 
		   "; 		   
	$sql2 = "
		   SELECT b.$field2 
		   FROM pdeescola.detalheplanoacao a
		   INNER JOIN pdeescola.$table b 
		   ON a.$field = b.$field
		   WHERE a.$field = {$_POST['saveId']}
	"; 		   
	$db->executar( $sql );
	$desc = $db->pegaUm( $sql2 );
	$db->commit();
	die( $desc );
}

if( $_POST['ajaxId'] != ''){
	
	header('content-type: text/html; charset=ISO-8859-1'); 
	
	if($_POST['tipo'] == 'programa'){
		$sql = "SELECT 
					tprid as codigo,
					tprdescricao as descricao
				FROM pdeescola.tipoprograma";
		
		$tprid = $_POST['ajaxId'];		 
        die( $db->monta_combo("tprid",$sql,'S',"Selecione...",'salvar('.$_POST["dpaid"].',this.value,'.$_POST["indice"].',\'programa\');','', '', 150, 'S', 'tprid') );
	}
	elseif( $_POST['tipo'] == 'categoria') {
		$sql = "SELECT 
					--tcaid||'_'||treid as codigo,
					tcaid as codigo,
					tcadescricao as descricao
				FROM pdeescola.tipocategoria ";

		$tcaid = $_POST['ajaxId']; 
		die( $db->monta_combo("tcaid",$sql,'S',"Selecione...",'salvar('.$_POST["dpaid"].',this.value,'.$_POST["indice"].',\'categoria\');','', '', 150, 'S', 'tcaid') );
	}
}

$cDado = array("instrumento" => 1);
$cForm = new formulario($cDado);
$cForm->direcPag('salva()');

include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
monta_titulo('Plano de Ação ', '');
echo cabecalhoPDE(); 

	$sql  = "SELECT * FROM pdeescola.planosuporteestrategico WHERE pdeid = '{$_SESSION['pdeid']}' AND pseidpai IS NULL ORDER BY pseid";
	  
	$objetivo = $db->carregar( $sql  );
	$objetivo = $objetivo ? $objetivo : array();
	
	
	if($objetivo){
		foreach( $objetivo as $arObjetivo ){
		
			$sql = "SELECT * FROM pdeescola.planosuporteestrategico    
		    		WHERE pseidpai = '{$arObjetivo['pseid']}'
					ORDER BY pseid ";
		                     
			$estrategia = $db->carregar( $sql );
			$estrategia = $estrategia ? $estrategia : array();
			
			foreach( $estrategia as $arEstrategia ){
		
				$sql = "SELECT * FROM pdeescola.planosuporteestrategico    
						WHERE pseidpai = '{$arEstrategia['pseid']}'
						ORDER BY pseid ";
            
				$meta = $db->carregar( $sql );
				$meta = $meta ? $meta : array();
				
				foreach( $meta as $arMeta ){
					
					$sql = "SELECT
											pa.plaid,
											pa.pseid,
											ps.pseidpai, 
											pa.plaliderobjetivo,
								            pa.plaindicadormeta,
								            pa.plagerenteplanoacao,
								            to_char(pa.pladatainicio::date,'MM/YYYY') as pladatainicio,
								            tr.tirdescricao,
								            to_char(pa.pladatatermino::date,'MM/YYYY') as pladatatermino,
								            ps.psedescricao
								         FROM
								         	pdeescola.planoacao AS pa
								         INNER JOIN 
								         	pdeescola.planosuporteestrategico AS ps ON ps.pseid = pa.pseid
								         INNER JOIN
								         	pdeescola.tiporevisao AS tr ON pa.tirid = tr.tirid
								         WHERE 
										    --ps.pdeid = '$pdeid'
										    --and 
										    ps.pseid = '{$arMeta["pseid"]}'
										 ORDER BY ps.pseid";

						$rsDadosMetas = $db->pegaLinha($sql);
						
						$msg = "Não informado";
						
						$plaid 				 = $rsDadosMetas['plaid'];
						$pseid 				 = $rsDadosMetas['pseid'];
						$plaliderobjetivo 	 = $rsDadosMetas['plaliderobjetivo'] ? $rsDadosMetas['plaliderobjetivo'] : $msg ;
						$plaindicadormeta 	 = $rsDadosMetas['plaindicadormeta'] ? $rsDadosMetas['plaindicadormeta'] : $msg ;
						$plagerenteplanoacao = $rsDadosMetas['plagerenteplanoacao'] ? $rsDadosMetas['plagerenteplanoacao'] : $msg;
						$pladatainicio		 = $rsDadosMetas['pladatainicio'] ? $rsDadosMetas['pladatainicio'] : $msg ;
						$pladatarevisao 	 = $rsDadosMetas['tirdescricao'] ? $rsDadosMetas['tirdescricao'] : $msg ;
						$pladatatermino 	 = $rsDadosMetas['pladatatermino'] ? $rsDadosMetas['pladatatermino'] : $msg ;					
						
						monta_titulo('', '');		
?>
		<br></br>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<script src="../includes/prototype.js"></script>
		    
		<table  align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" style="margin-bottom: 10px;">
			<tr>
				<td class="subtitulodireita" width="20%"><b>Objetivo Estratégico:</b></td> 
				<td align="left"><?php echo $arObjetivo["psedescricao"]; ?></td>
		   </tr>	
		   <tr>
				<td class="subtitulodireita"><b>Líder do Objetivo:</b></td> 
				<td align="left"><?php echo $plaliderobjetivo; ?></td>
			</tr>	
			<tr>
				<td class="subtitulodireita"><b>Estratégia:</b></td> 
				<td align="left"><?php echo $arEstrategia["psedescricao"]; ?> </td>
			</tr>	 
			<tr>
				<td class="subtitulodireita"><b>Meta:</b></td> 
				<td align="left"><?php echo $arMeta["psedescricao"]; ?> </td>
			</tr>	 
			<tr>
				<td class="subtitulodireita"><b>Indicador da Meta:</b> </td> 
				<td align="left"> <?php echo $plaindicadormeta; ?></td>
			</tr> 
			<tr>
				<td class="subtitulodireita"> <b>Gerente de Plano de Ação:</b> </td> 
				<td align="left"> <?php echo $plagerenteplanoacao; ?></td>
			</tr>	
		    <tr>
				<td class="subtitulodireita"> <b>Início:</b> </td> 
				<td align="left"> <?php echo $pladatainicio; ?> </td>
		    </tr>	
		    <tr>
				<td class="subtitulodireita"> <b>	Revisão:</b> </td> 
				<td align="left"> <?php echo $pladatarevisao; ?> </td>
		    </tr>	
		    <tr>
				<td class="subtitulodireita"> <b>	Data Término:</b> </td> 
				<td align="left"> <?php echo $pladatatermino; ?></td>
			</tr>	
		</table>
		
		<?=subCabecalho(' Desdobramento das Metas em Planos de Ação &nbsp;|&nbsp; <a style="font-size: 11px; color: blue" href="pdeescola.php?modulo=principal/sintese_autoavaliacao/reportPaf&acao=A">PAF</a> ');?>
		
		 <?php
		 
		 	//$plaid = $plaid ? $plaid : 0;
		        $sql = "SELECT   
				        d.dpadescricaoacao,
				        d.dpaid,
				        to_char(dpadatainicio::date,'MM/YYYY') as dataini,
				        to_char(dpadatatermino::date,'MM/YYYY') as datafim,
		                       dparesponsavel,
		                       --substr(d.dparesultadoesperado, 1, 100)||'...' as resultado,
		                        dparesultadoesperado,
		                       --substr(d.dpaindicador, 1, 100)||'...' as indicador,
		                       dpaindicador,
						d.dpavalorcapital,
						d.dpavalorcusteio,
		                       f.fordescricao,
		                       p.tprdescricao,
		                       p.tprid,
		                       t.tcaid,
		                       t.tcadescricao
					FROM 
						pdeescola.detalheplanoacao as d
					INNER JOIN 
						pdeescola.fonterecurso as f on f.forid = d.forid and d.forid = 5
					LEFT JOIN 
						pdeescola.tipocategoria as t on t.tcaid = d.tcaid
					LEFT JOIN 
						pdeescola.tipoprograma as p on p.tprid = d.tprid
					WHERE 
						plaid = '$plaid'
					ORDER BY 
						d.dpaid asc
					";
			if($plaid != '' ){			
			$dados = $db->carregar( $sql );
			}
		?>
		
	
	<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" style="color:333333;" class="listagem">
		<thead>
		<tr>
			<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
				Nº
			</td> 
			<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
				Descrição
			</td>
			<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
				Início
			</td>
			<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
				Término
			</td>
			<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
				Responsável
			</td>
			<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
				Resultado Esperado
			</td>
			<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
				Indicador
			</td>
			<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
				Capital (R$)
			</td>
			<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
				Custeio (R$)
			</td>
			<td align="center" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
				Quem Financia
			</td>
			<td align="center" width="15%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
				Tipo do Programa 
			</td>
			<td align="center" width="20%" valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">
				Categoria 
			</td>
		</tr> 
		</thead>
			<tbody>
				<?php
					//loop preenche Desdobramento das Metas em Planos de Ação
					for( $k = 0; $k < count( $dados ); $k++ ){
						//tratando lista caso não exista.
						if ($dados){
							?>
							<tr bgcolor="" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='';">
								<td title="Descrição">
									<b><?= $k+1 ?></b>
								</td> 
								<td title="Descrição">
									<?=$dados[$k]['dpadescricaoacao']?>
								</td>
								<td title="Início">
									<?=$dados[$k]['dataini']?>
								</td>
								<td title="Término">
									<?=$dados[$k]['datafim']?>
								</td>
								<td title="Responsável">
									<?=$dados[$k]['dparesponsavel']?>
								</td>
								<td title="Resultado Esperado">
									<?=$dados[$k]['dparesultadoesperado']?>
								</td>
								<td title="Indicador">
									<?=$dados[$k]['dpaindicador']?>
								</td>
								<td title="Capital (R$)" style="color:#3399FF;" title="Custeio (R$)">
									<?=$dados[$k]['dpavalorcapital']?>
								</td>
								<td align="right" style="color:#3399FF;" title="Custeio (R$)">
									<?=$dados[$k]['dpavalorcusteio']?>
								</td>
								<td title="Quem Financia">
									<?=$dados[$k]['fordescricao']?>
								</td>
								<td title="Tipo do Programa">
									
									<?php
									echo"<div id='programa[$k][{$dados[$k]['dpaid']}]'>";
									echo $dados[$k]['tprdescricao'];
									$perfis = arrayPerfil(); 
								    if( ( in_array(PDEESC_PERFIL_EQUIPE_TECNICA_MEC, $perfis ) ) || ( in_array(PDEESC_PERFIL_SUPER_USUARIO, $perfis ) ) ){					
										if( $dados[$k]['tprid'] != '' ) {
											echo"<br><a style='color: blue; text-decoration: underline; cursor: pointer;' onclick='alterar( {$dados[$k]['tprid']},  $k, \"programa\", {$dados[$k]['dpaid']});'> alterar</a>";
										}
								    }
									echo"</div>";		
									?>	
								</td>
								<td title="Categoria">
									
									<?php
									echo"<div id='categoria[$k][{$dados[$k]['dpaid']}]'>";
									echo $dados[$k]['tcadescricao'];
									
									$perfis = arrayPerfil(); 
								    if( ( in_array(PDEESC_PERFIL_EQUIPE_TECNICA_MEC, $perfis ) ) || ( in_array(PDEESC_PERFIL_SUPER_USUARIO, $perfis ) ) ){
										if( $dados[$k]['tcaid'] != '' ) {
											echo"<br><a style='color: blue; text-decoration: underline; cursor: pointer;' onclick='alterar( {$dados[$k]['tcaid']}, $k , \"categoria\", {$dados[$k]['dpaid']});'> alterar</a>";
										}
								    }
									echo"</div>";	
									?>	
								</td>
							</tr>
				    <?php
						}else{
							echo "<table width='95%' align='center' border='0' cellspacing='0' cellpadding='2' style='color:333333;' class='listagem'>
									<thead>
										<tr>
											<td style='color:red;' align='center';>
												Não foram encontrados Registros.
											</td>
										</tr>
								  </table>";
							continue;	 	
						}
					}
				}//fim foreach meta 
	    	}//fim foreach estrategia
  	  	}//fim foreach objetivo
	}else{//fim if ($objetivo)
		die('<script>
				 alert(\'Não foram encontrados Registros.\');
			 	history.go(-1);
	 		 </script>');
    }
?>
			</tbody>
	</table>
	<br></br>
	<div align="center">
		<input type="button" value="Voltar" onClick="location.href='pdeescola.php?modulo=principal/sintese_autoavaliacao/objetivos_estrategias_metas&acao=A'" />
	</div>
         
<script>
function alterar( id, indice, tipo, dpaid )
{
	var div = document.getElementById(tipo+'['+indice+']['+dpaid+']');
 	var req = new Ajax.Request('pdeescola.php?modulo=principal/sintese_autoavaliacao/consultaPlanoDeAcaoReport&acao=A', 
 	{
        method:     'post',
        parameters: '&ajaxId=' + id +'&tipo='+tipo +'&dpaid='+dpaid +'&indice='+indice,							         
        onComplete: function (res)
        {	
        	 div.innerHTML = res.responseText; 
        }
     
    });	
	
} 

function salvar( dpaid, id, indice, tipo)
{
	if( id == '' ){
		return false;
	} 
 	var div = document.getElementById(tipo+'['+indice+']['+dpaid+']');
 
	var req = new Ajax.Request('pdeescola.php?modulo=principal/sintese_autoavaliacao/consultaPlanoDeAcaoReport&acao=A', 
	{
        method:     'post',
        parameters: '&saveId='+id +'&dpaid='+dpaid +'&tipo='+tipo,							         
        onComplete: function (res)
        {	 
        	var desc = res.responseText;
        	var content = desc+"<br><a style=\"color: blue; text-decoration: underline; cursor: pointer;\" onclick=\"alterar("+id+","+indice+",'"+tipo+"',"+dpaid+");\">alterar</a>";
        	div.innerHTML = content;
        }
							        
	});
}
</script>
