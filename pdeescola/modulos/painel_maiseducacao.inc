<?

function pegaQtdPorAnoETipo($dados){
	
	global $db;
	
	$where = "AND me.memclassificacaoescola = '".$dados['tipo']."'";
	
	if( $dados['tipo'] == 'A' ){
		$where = "AND me.mamescolaaberta = 't'";
	}
	
	if( $dados['anoRel'] == 2011 ){
		$sql = "SELECT  
					count(*) as count
				FROM 
					pdeescola.memaiseducacao me
				INNER JOIN pdeescola.memaiseducacao me1 on me1.entid = me.entid and me1.memanoreferencia = ".$dados['anoAnterior']." and me1.memstatus = 'A'
				INNER JOIN entidade.endereco ent ON ent.entid = me.entid
				INNER JOIN territorios.estado est ON upper(est.estuf) = upper(ent.estuf)
				LEFT  JOIN workflow.documento d on d.docid=me.docid
				LEFT  JOIN workflow.estadodocumento ed on ed.esdid=d.esdid
				WHERE
					me.memanoreferencia = ".$dados['ano']." 
					AND me.memmodalidadeensino = 'F'
					AND me.memstatus = 'A'
					$where";
	}

	if( $dados['anoRel'] == 2012 ){
		$sql = "SELECT  
					count(*) as count
				FROM 
					pdeescola.memaiseducacao me
				INNER JOIN entidade.endereco ent ON ent.entid = me.entid
				INNER JOIN territorios.estado est ON upper(est.estuf) = upper(ent.estuf)
				LEFT  JOIN workflow.documento d on d.docid=me.docid
				LEFT  JOIN workflow.estadodocumento ed on ed.esdid=d.esdid
				WHERE
					me.entcodent not in (select mem.entcodent from pdeescola.memaiseducacao mem where mem.memanoreferencia = ".$dados['anoAnterior']." and mem.memstatus = 'A')
					AND me.memanoreferencia = ".$dados['ano']." 
					AND me.memmodalidadeensino = 'F'
					AND me.memstatus = 'A'
					$where";			
	}
	
	$qtd = $db->pegaUm($sql);
	return $qtd;
}

function montaPainel( $dados ){
	
	global $db;
	
	$ano = $dados['ano'];
	$anoAnterior = $dados['anoAnterior'];
	$anoMaximo = $db->pegaUm("select max(prsano) from pdeescola.programacaoexercicio");
	/*
	$where = "AND me.memclassificacaoescola = '".$dados['tipo']."'";
	
	if( $dados['tipo'] == 'A' ){
		$where = "AND me.mamescolaaberta = 't'";
	}
	*/
	
	if( (int)$dados['anoRel'] == (int)$_SESSION["exercicio"] ){
		$innerAno1 = "";
		$whereAno1 = "me.entcodent NOT IN (SELECT mem.entcodent FROM pdeescola.memaiseducacao mem WHERE mem.memanoreferencia = ".$anoAnterior." AND mem.memstatus = 'A')";
		//$whereAno1 = "1=1";
		$anoLink = $ano;
	}
	else{
		//$innerAno1 = "INNER JOIN pdeescola.memaiseducacao me1 ON me1.entid = me.entid AND me1.memanoreferencia = ".$anoAnterior." AND me1.memstatus = 'A'";
		//$whereAno1 = "1=1";
		$whereAno1 = "me.entcodent IN (SELECT mem.entcodent FROM pdeescola.memaiseducacao mem WHERE mem.memanoreferencia = ".$anoAnterior." AND mem.memstatus = 'A')";
		$anoLink = $ano;
		//$anoLink = $anoAnterior;
	}
	/*
	if( $dados['anoRel'] == 2012 ){
		$innerAno1 = "";
		$whereAno1 = "me.entcodent NOT IN (SELECT mem.entcodent FROM pdeescola.memaiseducacao mem WHERE mem.memanoreferencia = ".$anoAnterior." AND mem.memstatus = 'A')";
		$anoLink = $ano;
	}
	if( $dados['anoRel'] == 2011 ){
		
	}
	*/
	
	
	//$anoLink = $dados['anoRel'];
	//$whereAno1 = "1=1";
	
 	$sql = "SELECT 
 				CASE WHEN ed.esddsc is null then 'Não Iniciado' ELSE ed.esddsc END as esddsc, 
    			CASE WHEN ed.esdid IS NOT NULL THEN ed.esdid ELSE 0 END AS esdid, 
            	count(*) 
			FROM  
				pdeescola.memaiseducacao me
			$innerAno1
			INNER JOIN entidade.endereco ent ON ent.entid = me.entid
			INNER JOIN territorios.estado est ON upper(est.estuf) = upper(ent.estuf)
			LEFT JOIN workflow.documento d on d.docid=me.docid
			LEFT JOIN workflow.estadodocumento ed on ed.esdid=d.esdid 
			WHERE 
				$whereAno1
				AND me.memanoreferencia = ".$ano." 
				AND me.memmodalidadeensino = 'F'
				AND me.memstatus = 'A'
				$where
			GROUP BY 
				ed.esddsc, ed.esdordem, ed.esdid
			ORDER BY 
				ed.esdordem";
				
	$sql2 = "SELECT  
				est.estdescricao as descricao,
				d.esdid as esdid,
				est.estuf as estuf,
				count(*) as count
			FROM 
				pdeescola.memaiseducacao me
			$innerAno1
			INNER JOIN entidade.endereco ent ON ent.entid = me.entid
			INNER JOIN territorios.estado est ON upper(est.estuf) = upper(ent.estuf)
			LEFT JOIN workflow.documento d on d.docid=me.docid
			LEFT JOIN workflow.estadodocumento ed on ed.esdid=d.esdid
			WHERE
				$whereAno1
				AND d.esdid |agrupador|
				AND me.memanoreferencia = ".$ano." 
				AND me.memmodalidadeensino = 'F'
				AND me.memstatus = 'A'
				$where
			GROUP BY
				d.esdid,
				est.estuf,
				descricao
			ORDER BY
				descricao";
// 	ver($sql, $sql2, d);
	//$sqlAgrupador = array("sql" => $sql2, "agrupador" => "esdid", "link" => "pdeescola.php?modulo=melista&acao=E&modalidade=F", "campo" => array("esddsc","estdescricao"), "get" => array("esdid","estuf"), "arrOff" => array("estuf"), "exibeLink" => array("descricao"));
	//$link = array("link" => "pdeescola.php?modulo=melista&acao=E&modalidade=F&classificacao=U&memanoreferencia=".$anoLink."", "campo" => "esddsc", "get" => "esdid");
	
	$sqlAgrupador = array("sql" => $sql2, "agrupador" => "esdid", "link" => "pdeescola.php?modulo=melista&acao=E&modalidade=F&memanoreferencia=".$anoLink."", "campo" => array("esddsc","estdescricao"), "get" => array("esdid","estuf"), "arrOff" => array("estuf"), "exibeLink" => array("descricao"));
	$link = array("link" => "pdeescola.php?modulo=melista&acao=E&modalidade=F&memanoreferencia=".$anoLink."", "campo" => "esddsc", "get" => "esdid");
	
	if( pdeescola_possui_perfil(PDEESC_PERFIL_CONSULTA)  || $db->testa_superuser() ){
		listaSituacaoPorUF($dados['nomeTabela'],$sql,'',$cabecalho,$sqlAgrupador,"S",$link);
   	}
}

if( $_REQUEST['reqJson'] ){
	$dados = json_decode(str_replace('*','"',$_REQUEST['dados']),true);
	$_REQUEST['reqJson']($dados);
	die();
}

ini_set('memory_limit','1024M');
if( ( $_POST['act'] == 'listarUfByEstado') && ( $_POST['esdid'] != '' && ($_POST['esdid'] != 0)) ){
	//echo listarEstadoAjax( $_POST['esdid'] );
	$sql = "SELECT 
				DISTINCT
				estdescricao as descricao,
				est.esdid as esdid,
				t.estuf as estuf,
				count(*) as count
			FROM 
				entidade.entidade e
			INNER JOIN 
				entidade.entidadedetalhe ed ON ed.entid = e.entid and ed.entpdeescola = 't'
			LEFT JOIN 
				pdeescola.pdeescola pde ON pde.entid = e.entid AND pde.pdeano = 2008
			LEFT JOIN 
				territorios.estado t ON upper(pde.pdeuf) = upper(t.estuf)
			LEFT JOIN 
				workflow.documento d ON d.docid = pde.docid
			LEFT JOIN 
				workflow.estadodocumento est ON est.esdid = d.esdid
			WHERE
				d.esdid  = {$_POST['esdid']}
			AND
				estdescricao IS NOT NULL
			AND 
				pde.pdepafretorno IS NULL
			GROUP BY
				est.esdid,
				t.estuf,
				descricao
			ORDER BY
				descricao";
	 
	$rsEstado = $db->carregar( $sql, null, 3600 ); 
	
 	$tabela = '<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';    
	 if(!$rsEstado){
	 	$tabela .= "<tr><td align=center ><span style=\"color:#990000\" >Não existem Registros.</span></td></tr></table>";
	 	echo $tabela; 
	 	return false;
	 }else{
	 	for( $i =0; $i<count( $rsEstado ); $i++ ){
	 		$seta_filho = "<img  src=\"../imagens/seta_filho.gif\" />";
	 		 ($i % 2)? $cor = "#F7F7F7" : $cor = "#FCFCFC"; 
	 		$tabela .= "<tr bgcolor='$cor' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='$cor'\" >";
	 		$tabela .= "	<td width =\"80%\"> ".$seta_filho."<a href=\"pdeescola.php?modulo=lista&acao=E&esdid=".$rsEstado[$i]['esdid']."&estuf=".$rsEstado[$i]['estuf']."\">".$rsEstado[$i]['descricao']."</a></td>";
	 		$tabela .= "	<td style=\"color:rgb(0, 102, 204);text-align:right\"> ".str_replace(",",".",number_format($rsEstado[$i]['count']))."</td>";
	 		$tabela .= "</tr>"; 
	 	}
	 	$tabela .= "</table>";
	 }
	 echo $tabela;
	die();
}else if(( $_POST['act'] == 'listarUfByEstado') && ( $_POST['esdid'] == 0)){
	//echo listarEstadoAjax( $_POST['esdid'] );
	$sql = "SELECT 
				est.estdescricao as descricao,
				coalesce(ed.esdid,0) as esdid,
				est.estuf as estuf,
				count(*) as count
			FROM 
				entidade.entidade ent
			LEFT JOIN
				pdeescola.pdeescola pde ON ent.entid = pde.entid
			INNER JOIN 
				entidade.entidadedetalhe det on det.entid = ent.entid
			INNER JOIN 
				entidade.endereco entEnd on det.entid = entEnd.entid
			INNER JOIN
				territorios.estado est ON upper(entEnd.estuf) = upper(est.estuf)
			LEFT JOIN 
				workflow.documento d on d.docid = pde.docid
			LEFT JOIN 
				workflow.estadodocumento ed on ed.esdid = d.esdid
			WHERE
				d.esdid is null
			AND
				det.entpdeescola = 't'
			GROUP BY
				ed.esdid,
				est.estuf,
				descricao
			ORDER BY
				descricao";

	$rsEstado = $db->carregar( $sql, null, 3600 ); 
	
 	$tabela = '<table width="100%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';    
	 if(!$rsEstado){
	 	$tabela .= "<tr><td align=center ><span style=\"color:#990000\" >Não existem Registros.</span></td></tr></table>";
	 	echo $tabela; 
	 	return false;
	 }else{
	 	for( $i =0; $i<count( $rsEstado ); $i++ ){
	 		$seta_filho = "<img  src=\"../imagens/seta_filho.gif\" />";
	 		 ($i % 2)? $cor = "#F7F7F7" : $cor = "#FCFCFC"; 
	 		$tabela .= "<tr bgcolor='$cor' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='$cor'\" >";
	 		$tabela .= "	<td width =\"80%\"> ".$seta_filho."<a href=\"pdeescola.php?modulo=lista&acao=E&esdid=".$rsEstado[$i]['esdid']."&estuf=".$rsEstado[$i]['estuf']."\">".$rsEstado[$i]['descricao']."</a></td>";
	 		$tabela .= "	<td style=\"color:rgb(0, 102, 204);text-align:right\"> ".str_replace(",",".",number_format($rsEstado[$i]['count']))."</td>";
	 		$tabela .= "</tr>"; 
	 	}
	 	$tabela .= "</table>";
	 }
	 echo $tabela;
	die();	
}

	
function listarUfByEstado(){
	global $db;
	$sql = "(SELECT * FROM (
				 (SELECT 
				 	CASE  
				 	WHEN est.esddsc IS NOT NULL THEN est.esddsc
				       ELSE 'Não Iniciado'
				    END AS esddsc,
				    CASE   WHEN est.esdid IS NOT NULL THEN est.esdid
				       ELSE 0
				    END AS esdid,
				    count(*) as count,
				    est.esdordem
				    FROM 
				    	entidade.entidade e
				    INNER JOIN entidade.entidadedetalhe ed ON ed.entid = e.entid and ed.entpdeescola = 't'
				    LEFT JOIN pdeescola.pdeescola pde ON pde.entid = e.entid AND pde.pdeano = 2008
				    LEFT JOIN territorios.estado t ON upper(pde.pdeuf) = upper(t.estuf)
				    LEFT JOIN workflow.documento d ON d.docid = pde.docid
				    LEFT JOIN workflow.estadodocumento est ON est.esdid = d.esdid
				   
				    inner JOIN pdeescola.preenchimento pr ON pr.entid = pde.entid

				    WHERE
					pr.entcodent is not null
					AND est.esdid IS NULL
					OR est.esdid != '90'
					AND est.esdid != '87'

  					
				    GROUP BY est.esddsc, est.esdid, est.esdordem)
				   UNION
				  (SELECT 
				  	CASE 
				  	WHEN est.esddsc IS NOT NULL THEN est.esddsc
				       ELSE 'Não Iniciado'
				    END AS esddsc,
				    CASE WHEN est.esdid IS NOT NULL THEN est.esdid
				       ELSE 0
				    END AS esdid,
				    count(*) as count,
				    est.esdordem
				    FROM 
				  		entidade.entidade e
				    INNER JOIN entidade.entidadedetalhe ed ON ed.entid = e.entid and ed.entpdeescola = 't'
				    LEFT JOIN pdeescola.pdeescola pde ON pde.entid = e.entid AND pde.pdeano = 2008
				    LEFT JOIN territorios.estado t ON upper(pde.pdeuf) = upper(t.estuf)
				    LEFT JOIN workflow.documento d ON d.docid = pde.docid
				    LEFT JOIN workflow.estadodocumento est ON est.esdid = d.esdid

				    inner JOIN pdeescola.preenchimento pr ON pr.entid = pde.entid AND est.esdid != '76'
				    
				  WHERE pr.esdid = '87' or est.esdid = '90' 
				     GROUP BY est.esddsc, est.esdid, est.esdordem)
		         ) as pp
			ORDER BY pp.esdordem)";

	$rsEstado = $db->carregar( $sql, null, 3600 ); 
	$tabela .= '<table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem">';   
 	
	 if(!$rsEstado){
	 	$tabela = "<tr><td align=center ><span style=\"color:#990000\" >Não existem Registros.</span></td></tr></table>";
	 	echo $tabela; 
	 	return false;
	 }else{
	 	for( $i =0; $i<count( $rsEstado ); $i++ ){
	 		$seta_filho = "<img  src=\"../imagens/seta_filho.gif\" />";
	 		$img = "<img onclick=\"exibeUfAjax('".$rsEstado[$i]['esdid']."')\" style=\"cursor:pointer; display:'';\" id=\"imgMais_".$rsEstado[$i]['esdid']."\" align=\"abdmiddle\" src=\"../imagens/mais.gif\" title=\"Abrir\" /> 
	 				<img onclick=\"escondeUfAjax('".$rsEstado[$i]['esdid']."')\" style=\"cursor:pointer; display:none \" id=\"imgMenos_".$rsEstado[$i]['esdid']."\" align=\"abdmiddle\" src=\"../imagens/menos.gif\" title=\"Fechar\" /> ";				
	 		($i % 2)? $cor = "#F7F7F7" : $cor = "#FCFCFC"; 
	 		$tabela .= "<tr bgcolor='$cor' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='$cor'\" >";
	 		$tabela .= "	<td width =\"80%\"> ".$img."<a href=\"pdeescola.php?modulo=lista&acao=E&esdid=".$rsEstado[$i]['esdid']."\">".$rsEstado[$i]['esddsc']."</a>";
	 		$tabela .= "<div id='msg_".$rsEstado[$i]['esdid']."' style='display:none;'></div>"; 
	 		$tabela.="</td>";
	 		$tabela .= "	<td width =\"20%\" style=\"color:rgb(0, 102, 204);text-align:right\"> ".str_replace(",",".",number_format($rsEstado[$i]['count']))."</td>";
	 		$tabela .= "</tr>";
	 		$tabela .= "<tr id=\"tr_".$rsEstado[$i]['esdid']."\" style=\"display:none;\" >
	 						<td colspan=\"2\">
	 						<div id=\"subitem_".$rsEstado[$i]['esdid']."\" ></div>
	 						</td>
	 					</tr>";
	 	}
	 	$tabela .= "</table>";
	 }
	 return $tabela;
}
?>
<script type="text/javascript" src="/includes/prototype.js"></script>	
<script src="./js/meajax.js" type="text/javascript"></script>
<script>

function carregando(id){	
	var msg = document.getElementById('msg_'+id);
	msg.style.display="block";
	msg.innerHTML="<img src=\"../imagens/wait.gif\"'>";
}
function exibeUfAjax(id){
	if( id == ''){ 
			return false;
		}
		var tr  = document.getElementById('tr_'+id); 
		var td  = document.getElementById('subitem_'+id); 
		var imgMais = document.getElementById('imgMais_'+id);
		var imgMenos= document.getElementById('imgMenos_'+id);
	 	var msg = document.getElementById('msg_'+id);
		var req = new Ajax.Request('pdeescola.php?modulo=painel_maiseducacao&acao=A',
		                                               {
		                                                   method: 'post',
		                                                   parameters: 'esdid='+id+'&act=listarUfByEstado',
		                                                   onLoading: carregando(id),
		                                                   onComplete: function(res)
		                                                   {
		                                                   		td.innerHTML = res.responseText;        
		                                                   		tr.style.display='';
		                                                   		td.style.display=''; 
		                                                   		imgMais.style.display = 'none';
																imgMenos.style.display = ''; 
																msg.style.display ='none';
		                                                   }
		                                               }); 
}
function escondeUfAjax(id){
	var tr  = document.getElementById('tr_'+id); 
	var td  = document.getElementById('subitem_'+id); 
	var imgMais = document.getElementById('imgMais_'+id);
	var imgMenos= document.getElementById('imgMenos_'+id);
	tr.style.display='none';
	td.style.display='none'; 
	imgMais.style.display = '';
	imgMenos.style.display = 'none';
}
</script>
<?php

/*Função para montar lista com Agrupador e Links*/
function listaSituacaoPorUF($id = "tabela_1",$sql,$titulo = null,$cabecalho = null,$sqlAgrupador = array(),$exibeSoma = "S",$link = array(),$arrOff = array() ){
	 global $db;
	 //dbg($sql);
	 $dados = $db->carregar($sql, null, 3600);
 
	 $tabela = '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
	 
	 
	 if(!$dados){
	 	$tabela .= "<tr><td align=center ><span style=\"color:#990000\" >Não existem Registros.</span></td></tr></table>";
	 	echo $tabela; 
	 	return false;
	 }

	 $num_colunas = count($dados[0]);
	 $num_colunas = $num_colunas - (count($arrOff));
	 
	 if($titulo){
	 	$tabela .= "<tr bgcolor=#CCCCCC ><td colspan=\"$num_colunas\" align=center ><b>$titulo</b></td></tr>";
	 }
	 
	 if($cabecalho){
	 	$tabela .= "<tr bgcolor=#e9e9e9 onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='#e9e9e9'\" >";
		 $i = 0;
		 while($i < $num_colunas){
		 	$tabela .= "<td><b>".$cabecalho[$i]."</b></td>";
		 	$i++;
		 }
		 $tabela .= "</tr>";
	 }
	 $id_span = 1;
	 $i = 0;
	 foreach($dados as $d){
	 	
	 	$cor = ($i % 2) ? "#F7F7F7" : "#FCFCFC";
	 	
	 	$tabela .= "<tr bgcolor='$cor' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='$cor'\" >";
	 	
	 	$sqlAg = $sqlAgrupador['sql'];
	 	
	 	if($sqlAgrupador['sql']){
	 		if($sqlAgrupador['agrupador'] && $d[$sqlAgrupador['agrupador']] || $d[$sqlAgrupador['agrupador']] == "0" || $d[$sqlAgrupador['agrupador']] == "999999"  || $d[$sqlAgrupador['agrupador']] == "888888"){
	 			if($d[$sqlAgrupador['agrupador']] == "0" && $id == "tabela_1"){
	 				//$sqlAg = str_replace("|agrupador|"," is null ");
	 				
	 				$ano = $_SESSION["exercicio"];
					$anoAnterior = $ano -1;
	 				$sqlAg = "
							SELECT 
								est.estdescricao as descricao,
								coalesce(ed.esdid, 0) as esdid,
								est.estuf as estuf,
								count(*) as count
							FROM 
								entidade.entidade ent
							INNER JOIN
								pdeescola.memaiseducacao me ON ent.entid = me.entid
							INNER JOIN 
								entidade.entidadedetalhe det on det.entid = ent.entid
							INNER JOIN 
								entidade.endereco entEnd on det.entid = entEnd.entid
							INNER JOIN
								territorios.estado est ON upper(entEnd.estuf) = upper(est.estuf)
							LEFT JOIN 
								workflow.documento d on d.docid = me.docid
							LEFT JOIN 
								workflow.estadodocumento ed on ed.esdid = d.esdid
							where
								d.esdid is null
							AND me.entcodent not in (select mem.entcodent from pdeescola.memaiseducacao mem where mem.memanoreferencia = ".$anoAnterior." and mem.memstatus = 'A')
							AND me.memanoreferencia = ".$ano." 
							AND me.memmodalidadeensino = 'F'
							AND me.memstatus = 'A' 
							GROUP BY
								ed.esdid,
								est.estuf,
								descricao
							ORDER BY
								descricao";

	 			}elseif($d[$sqlAgrupador['agrupador']] == "0" && $id == "tabela_2"){
	 				//$sqlAg = str_replace("|agrupador|"," is null ");
	 				$ano = $_SESSION["exercicio"];
					$anoAnterior = $ano -1;
	 				$sqlAg = "
							SELECT 
								est.estdescricao as descricao,
								coalesce(ed.esdid, 0) as esdid,
								est.estuf as estuf,
								count(*) as count
							FROM 
								entidade.entidade ent
							INNER JOIN
								pdeescola.memaiseducacao me ON ent.entid = me.entid
							INNER JOIN 
								pdeescola.memaiseducacao me1 ON me1.entid = me.entid AND me1.memanoreferencia = ".$anoAnterior." AND me1.memstatus = 'A'
							INNER JOIN 
								entidade.entidadedetalhe det on det.entid = ent.entid
							INNER JOIN 
								entidade.endereco entEnd on det.entid = entEnd.entid
							INNER JOIN
								territorios.estado est ON upper(entEnd.estuf) = upper(est.estuf)
							LEFT JOIN 
								workflow.documento d on d.docid = me.docid
							LEFT JOIN 
								workflow.estadodocumento ed on ed.esdid = d.esdid
							where
								d.esdid is null
							AND me.memanoreferencia = ".$ano." 
							AND me.memmodalidadeensino = 'F'
							AND me.memstatus = 'A' 
							GROUP BY
								ed.esdid,
								est.estuf,
								descricao
							ORDER BY
								descricao";
	 				

	 			}elseif($d[$sqlAgrupador['agrupador']] == "0" && $id == "tabela_3"){
	 				//executa este sql quando esdid is null  --> Não iniciado --> $d[$sqlAgrupador['agrupador']] == "0"	
	 				$ano = $_SESSION["exercicio"];
					$anoAnterior = $ano -1; 
									
	 				$sqlAg = "
							SELECT  
								est.estdescricao as descricao,
								coalesce(ed.esdid, 0) as esdid,
								est.estuf as estuf,
								count(*) as count
							FROM 
								entidade.entidade ent
							INNER JOIN
								pdeescola.memaiseducacao me ON ent.entid = me.entid
							INNER JOIN
								entidade.entidadedetalhe det on det.entid = ent.entid
							INNER JOIN 
								entidade.endereco entEnd on det.entid = entEnd.entid
							INNER JOIN
								territorios.estado est ON upper(entEnd.estuf) = upper(est.estuf)
							LEFT JOIN 
								workflow.documento d on d.docid = me.docid
							LEFT JOIN 
								workflow.estadodocumento ed on ed.esdid = d.esdid
							WHERE
								d.esdid is null
							AND me.entcodent not in (select mem.entcodent from pdeescola.memaiseducacao mem where mem.memanoreferencia = ".$anoAnterior." and mem.memstatus = 'A')
							AND me.memanoreferencia = ".$ano."
							AND me.memmodalidadeensino = 'M'
							AND me.memstatus = 'A' 
							GROUP BY
								ed.esdid,
								est.estuf,
								descricao
							ORDER BY
								descricao";

	 			}elseif($d[$sqlAgrupador['agrupador']] == "999999" && $id == "tabela_4"){
	 				
	 				$sqlAg = "SELECT
									est.estdescricao as descricao,
									coalesce(we.esdid,999999) as esdid,
									est.estuf as estuf,
									count(*) as count
								FROM 
									entidade.entidade e
								INNER JOIN 
									entidade.entidadedetalhe ed ON ed.entid = e.entid 
								INNER JOIN
									pdeescola.pdeescola pde ON pde.entid = e.entid and pde.pdeano = ".ANO_EXERCICIO_PDE_ESCOLA."
								INNER JOIN 
									workflow.documento d ON d.docid = pde.docid
								LEFT OUTER JOIN 
									workflow.estadodocumento we ON we.esdid = d.esdid AND we.esdid is null
								INNER JOIN 
									entidade.endereco ee on ee.entid = e.entid
								INNER JOIN
									territorios.estado est ON upper(ee.estuf) = upper(est.estuf)
								WHERE
									ed.entpdeescola = 't'
								AND
									pde.pdepafretorno IS NOT NULL										
								GROUP BY
									we.esdid,
									est.estuf,
									descricao
								ORDER BY
									descricao";	
	 				
	 			}elseif($d[$sqlAgrupador['agrupador']] == "888888" && $id == "tabela_5"){

				$sqlAg = "SELECT
							est.estdescricao as descricao,
							coalesce(NULLIF(we.esdid, 90),888888) as esdid,
							est.estuf as estuf,
							count(*) as count
						FROM 
							entidade.entidade e
						INNER JOIN 
							entidade.entidadedetalhe ed ON ed.entid = e.entid 
						INNER JOIN
							pdeescola.pdeescola pde ON pde.entid = e.entid and pde.pdeano = ".ANO_EXERCICIO_PDE_ESCOLA."  
						INNER JOIN 
							pdeescola.preenchimento pp ON pp.entid = pde.entid 
						INNER JOIN 
							workflow.documento d ON d.docid = pde.docid
						INNER JOIN 
							workflow.estadodocumento we ON we.esdid = d.esdid
						INNER JOIN 
							entidade.endereco ee on ee.entid = e.entid
						INNER JOIN
							territorios.estado est ON upper(ee.estuf) = upper(est.estuf)
						WHERE ed.entpdeescola = 't'
						AND pde.pdeid in ( SELECT distinct a.pdeid
										   FROM  pdeescola.avaliacaoplano as a
										   LEFT JOIN  pdeescola.pdeescola as p on p.pdeid = a.pdeid 
										   LEFT JOIN  pdeescola.preenchimento as pp on pp.entid = p.entid AND pp.esdid = 90
										   GROUP BY a.pdeid
										   HAVING count(a.qapid) >= 55 )
						AND pde.pdepafretorno IS NOT NULL
						GROUP BY
							we.esdid,
							est.estuf,
							descricao
						ORDER BY
							descricao";

	 			}elseif($d[$sqlAgrupador['agrupador']] == "0" && $id == "tabela_6"){
	 				//$sqlAg = str_replace("|agrupador|"," is null ");
	 				$ano = $_SESSION["exercicio"];
					$anoAnterior = $ano -1;

					$sqlAg = "SELECT 
								est.estdescricao as descricao,
								coalesce(ed.esdid, 0) as esdid,
								est.estuf as estuf,
								count(*) as count
							FROM 
								pdeescola.memaiseducacao me 
							INNER JOIN 
								pdeescola.memaiseducacao me1 on me1.entid = me.entid and me1.memanoreferencia = ".$anoAnterior." and me1.memstatus = 'A'
							INNER JOIN 
								entidade.entidadedetalhe det on det.entid = me.entid
							INNER JOIN 
								entidade.endereco entEnd on det.entid = entEnd.entid
							INNER JOIN
								territorios.estado est ON upper(entEnd.estuf) = upper(est.estuf)
							LEFT JOIN 
								workflow.documento d on d.docid = me.docid
							LEFT JOIN 
								workflow.estadodocumento ed on ed.esdid = d.esdid
							WHERE
								d.esdid is null
							AND 
								me.memanoreferencia = ".$ano." 
							AND 
								me.memmodalidadeensino = 'F'
							AND 
								me.memstatus = 'A' 
							GROUP BY
								ed.esdid,
								est.estuf,
								descricao
							ORDER BY
								descricao";	
	 			}elseif((($d[$sqlAgrupador['agrupador']] == "0") || ($d[$sqlAgrupador['agrupador']] == null)) && ($id == "tabela_7")){
	 				//executa este sql quando esdid is null  --> Não iniciado --> $d[$sqlAgrupador['agrupador']] == "0"
	 				$ano = $_SESSION["exercicio"];
					$anoAnterior = $ano -1;	 				
	 				$sqlAg = "
							SELECT  
								est.estdescricao as descricao,
								coalesce(ed.esdid, 0) as esdid,
								est.estuf as estuf,
								count(*) as count
							FROM 
								pdeescola.memaiseducacao me 
							INNER JOIN 
								pdeescola.memaiseducacao me1 on me1.entid = me.entid and me1.memanoreferencia = ".$anoAnterior." and me1.memstatus = 'A'
							INNER JOIN 
								entidade.entidadedetalhe det on det.entid = me.entid
							INNER JOIN 
								entidade.endereco entEnd on det.entid = entEnd.entid
							INNER JOIN
								territorios.estado est ON upper(entEnd.estuf) = upper(est.estuf)
							LEFT JOIN 
								workflow.documento d on d.docid = me.docid
							LEFT JOIN 
								workflow.estadodocumento ed on ed.esdid = d.esdid
							WHERE
								d.esdid is null
							AND 
								me.memanoreferencia = ".$ano."  
							AND 
								me.memmodalidadeensino = 'M'
							AND 
								me.memstatus = 'A' 
							GROUP BY
								ed.esdid,
								est.estuf,
								descricao
							ORDER BY
								descricao";				
	 			}
	 			
	 			
	 			
	 		elseif((($d[$sqlAgrupador['agrupador']] == "0") || ($d[$sqlAgrupador['agrupador']] == null)) && ($id == "tabela_7")){
	 				//executa este sql quando esdid is null  --> Não iniciado --> $d[$sqlAgrupador['agrupador']] == "0"
	 				$ano = $_SESSION["exercicio"];
					$anoAnterior = $ano -1;	 				
	 				$sqlAg = "
							SELECT  
								est.estdescricao as descricao,
								coalesce(ed.esdid, 0) as esdid,
								est.estuf as estuf,
								count(*) as count
							FROM 
								pdeescola.memaiseducacao me 
							INNER JOIN 
								pdeescola.memaiseducacao me1 on me1.entid = me.entid and me1.memanoreferencia = ".$anoAnterior." and me1.memstatus = 'A'
							INNER JOIN 
								entidade.entidadedetalhe det on det.entid = me.entid
							INNER JOIN 
								entidade.endereco entEnd on det.entid = entEnd.entid
							INNER JOIN
								territorios.estado est ON upper(entEnd.estuf) = upper(est.estuf)
							LEFT JOIN 
								workflow.documento d on d.docid = me.docid
							LEFT JOIN 
								workflow.estadodocumento ed on ed.esdid = d.esdid
							WHERE
								d.esdid is null
							AND 
								me.memanoreferencia = ".$ano."  
							AND 
								me.memmodalidadeensino = 'M'
							AND 
								me.memstatus = 'A' 
							GROUP BY
								ed.esdid,
								est.estuf,
								descricao
							ORDER BY
								descricao";				
	 			}
	 			
	 			/*** Escola Acessível ***/
	 			elseif((($d[$sqlAgrupador['agrupador']] == "0") || ($d[$sqlAgrupador['agrupador']] == null)) && ($id == "tabela_8")){
	 									
	 				$sqlAg = "
							SELECT  
								est.estdescricao as descricao,
								coalesce(ed.esdid, 0) as esdid,
								est.estuf as estuf,
								count(*) as count
							FROM 
								pdeescola.eacescolaacessivel eac
							INNER JOIN 
								entidade.entidadedetalhe det on det.entid = eac.entid
							INNER JOIN 
								entidade.endereco entEnd on det.entid = entEnd.entid
							INNER JOIN
								territorios.estado est ON upper(entEnd.estuf) = upper(est.estuf)
							LEFT JOIN 
								workflow.documento d on d.docid = eac.docid
							LEFT JOIN 
								workflow.estadodocumento ed on ed.esdid = d.esdid
							WHERE
								d.esdid is null
							AND 
								eac.eacanoreferencia = ".$_SESSION["exercicio"]."  
							AND 
								eac.eacstatus = 'A' 
							GROUP BY
								ed.esdid,
								est.estuf,
								descricao
							ORDER BY
								descricao";				
	 			}
	 			else{
	 				
	 				if($d[$sqlAgrupador['agrupador']] == "0"){
	 					$sqlAg = str_replace("|agrupador|"," is null ",$sqlAg);
	 				}
	 				else{
	 					$sqlAg = str_replace("|agrupador|"," = ".$d[$sqlAgrupador['agrupador']],$sqlAg);
	 				}
	 			}
	 			//dbg($sqlAg);
// 	 			$dadosAgrupados = $db->carregar($sqlAg);
	 			$dadosAgrupados = false;

	 		}else{
	 			$dadosAgrupados = "";
	 		}
	 		
	 		
	 		$listaAgrupada = '<table cellspacing="0" cellpadding="2" border="0" align="center" width="100%" class="listagem">';
	 		
	 		if(!$dadosAgrupados){
	 			$listaAgrupada .= "<tr><td><span style=\"color:#990000\" >Não existem registros.</span></td></tr>";
	 		}else{
	 			$xx = 0;
	 			foreach($dadosAgrupados as $dA){
	 				($xx % 2)? $cor = "#F7F7F7" : $cor = "#FCFCFC";
	 				$listaAgrupada .= "<tr bgcolor='$cor' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='$cor'\" >";

	 				foreach($dA as $k => $dd){
	 					$kk[] = $k;
	 				}
	 				$ii = 0;			
	 				while($ii < count($dA)){
	 					
	 					if($sqlAgrupador['link']){
	 						if($sqlAgrupador['campo']){
	 							if(is_array($sqlAgrupador['campo'])){
	 								unset($arrCampos);
	 								foreach($sqlAgrupador['get'] as $cmp){
	 									$arrCampos[] = "{$cmp}={$dA[$cmp]}";
	 									$campos = implode("&",$arrCampos);
	 								}
	 							}else{
	 								$campos = "{$sqlAgrupador['get']}={{$dA[$kk[$sqlAgrupador['get']]]}}"; 
	 							}
	 						}	 						
	 						$linkAg_a = "<a href=\"".$sqlAgrupador['link']."&".$campos."\" />"; 
	 						$linkAg_b = " </a>";
	 					}
	 					if($kk[$ii] == $kk[0]){
	 						$seta_filho = "<img src=\"../imagens/seta_filho.gif\" />";
	 					}else{
	 						$seta_filho = "";
	 					}

	 					if(!strstr($kk[$ii],"id") && !strstr($kk[$ii],"ordem") && !in_array($kk[$ii],$sqlAgrupador['arrOff'])){
	 					
	 						if(in_array($kk[$ii],$sqlAgrupador['exibeLink'])){
	 						
			 					if(is_numeric($dA[$kk[$ii]])){
							 		$campo = str_replace(",",".",number_format($dA[$kk[$ii]]));
							 		$listaAgrupada .= "<td align=\"right\"><span style=\"color:rgb(0, 102, 204);text-align:right\" >$seta_filho $linkAg_a $campo $linkAg_b</span></td>";
			 					}
							 	else{
							 		$listaAgrupada .= "<td>$seta_filho $linkAg_a {$dA[$kk[$ii]]} $linkAg_b</td>";
							 	}
	 						}
	 						else{
	 							if(is_numeric($dA[$kk[$ii]])){
							 		$campo = str_replace(",",".",number_format($dA[$kk[$ii]]));
							 		$listaAgrupada .= "<td align=\"right\" ><span style=\"color:rgb(0, 102, 204);text-align:right;width:100%\" >$seta_filho $campo</span></td>";
			 					}
							 	else{
							 		$listaAgrupada .= "<td>$seta_filho {$dA[$kk[$ii]]}</td>";
							 	}
	 						}
						 	
	 					}
						$ii++;
	 					
	 				}
	 				$listaAgrupada .= "</tr>";
	 			$xx++;
	 			}
	 		}
	 		$listaAgrupada .= "</table>";
	 	}
	 	
	 	$keys = array_keys($d);
	 	$j = 0;
		while($j < $num_colunas){
			if($sqlAgrupador && $keys[$j] == $keys[0] || $keys[$j] == $keys[999999] || $keys[$j] == $keys[888888] && $dadosAgrupados){
				$img = "<img onclick=\"exibeAgrupador('{$id}_{$id_span}')\" style=\"cursor:pointer\" id=\"img_mais_{$id}_{$id_span}\" align=\"abdmiddle\" src=\"../imagens/mais.gif\" title=\"Abrir\" />
						<img onclick=\"escondeAgrupador('{$id}_{$id_span}')\" style=\"cursor:pointer;display:none\" id=\"img_menos_{$id}_{$id_span}\" align=\"abdmiddle\" src=\"../imagens/menos.gif\" title=\"Fechar\" /> ";
				$span = "<tr style=\"display:none\" bgcolor='#EEE9E9' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='#EEE9E9'\" id=\"tr_view_{$id}_{$id_span}\"><td colspan=\"$num_colunas\">$listaAgrupada</td></td></tr>";	
				$id_span ++; 
			}
			else{
				$img = "&nbsp;&nbsp;&nbsp;&nbsp;";
			}
						
			//Monta os links;
			if($link && $dadosAgrupados){
				$link_a = "<a href=\"{$link['link']}&{$link['get']}=".$d[$link['get']]."\" >";
				$link_b = "</a>";
			}else{
				$link_a = "";
				$link_b = "";
			}
			
			
			if(!strstr($keys[$j],"id") && !strstr($keys[$j],"ordem") && !in_array($keys[$j],$arrOff)){
				
				if(is_numeric($d[$keys[$j]])){
					$tabela .= "<td align=\"right\">";
				}else{
					$tabela .= "<td>";
				}
				
				if($link['campo'] == $keys[$j]){
					$tabela .= $img.$link_a;
				}else{
					$tabela .= $img;
				}
			 	if(is_numeric($d[$keys[$j]])){
			 		$campo = str_replace(",",".",number_format($d[$keys[$j]]));
			 		$tabela .= "<span style=\"color:rgb(0, 102, 204)\" >".$campo.$link_b."</span></td>";
			 	}else{
				 	if($link['campo'] == $keys[$j]){
						$tabela .= $d[$keys[$j]].$link_b."</td>";
					}else{
						$tabela .= $d[$keys[$j]]."</td>";
					}
			 		
			 	}

			}
		 	
		 	if(!strstr($keys[$j],"ordem") && is_numeric($d[$keys[$j]])  && !in_array($keys[$j],$arrOff)){
		 		$soma[$keys[$j]] += $d[$keys[$j]];
		 		$campo_soma[] = $keys[$j];
		 	}
		 	$j++;
		 	
		}
		
	 	$tabela .= "</tr>";
	 	$tabela .= $span;
	 	
	 	$i++;
	 }
	 
	 foreach($keys as $k => $k1){
	 	 if(strstr($k1,"id")){
	 	 	unset ($keys[$k]);
	 	 }
	 }
	 	 
	 //Exibe Soma
	 if($exibeSoma == "S"){
	 	$tabela .= "<tr bgcolor='DCDCDC' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='DCDCDC'\" >";
	 	$campo_soma = array_unique($campo_soma);
	 	foreach($keys as $k1 => $k){
	 		
	 		if(!in_array($k,$arrOff)){
	 		
		 		if(in_array($k,$campo_soma)){
		 			$tabela .= "<td align=\"right\" ><b>".str_replace(",",".",number_format($soma[$k]))."</b></td>";
		 		}elseif($k1 == 0){
		 			$tabela .= "<td><b>Total:</b></td>";
		 		}else{
		 			$tabela .= "<td></td>";
		 		}
	 		}
	 	}
	 	$tabela .= "</tr>";
	 }
	 if($exibeSoma == "X"){
	 	$tabela .= "<tr bgcolor='DCDCDC' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='DCDCDC'\" >";
	 	$campo_soma = array_unique($campo_soma);
	 	foreach($keys as $k1 => $k){
	 		
	 		if(!in_array($k,$arrOff)){
	 		
		 		if(in_array($k,$campo_soma)){
		 			$tabela .= "<td align=\"right\" ><b>".str_replace(",",".",number_format($soma[$k]))."</b></td>";
		 		}elseif($k1 == 0){
		 			$tabela .= "<td><b>Total:</b></td>";
		 		}else{
		 			$tabela .= "<td></td>";
		 		}
	 		}
	 	}
	 	$tabela .= "</tr>";
	 }
	 if($exibeSoma == "Y"){
	 	$tabela .= "<tr bgcolor='DCDCDC' onmouseover=\"this.bgColor='#ffffcc'\" onmouseout=\"this.bgColor='DCDCDC'\" >";
	 	$campo_soma = array_unique($campo_soma);
	 	foreach($keys as $k1 => $k){
	 		
	 		if(!in_array($k,$arrOff)){
	 		
		 		if(in_array($k,$campo_soma)){
		 			$tabela .= "<td align=\"right\" ><b>".str_replace(",",".",number_format($soma[$k]))."</b></td>";
		 		}elseif($k1 == 0){
		 			$tabela .= "<td><b>Total:</b></td>";
		 		}else{
		 			$tabela .= "<td></td>";
		 		}
	 		}
	 	}
	 	$tabela .= "</tr>";
	 }
	 	 
	 $tabela .= "</table>";
	 $tabela .="<script>
	 function exibeAgrupador(id){
	 	var img_mais = document.getElementById('img_mais_' +id);
	 	var img_menos = document.getElementById('img_menos_' +id);
	 	var tr_view = document.getElementById('tr_view_' +id);
	 	
	 	img_mais.style.display = 'none';
	 	img_menos.style.display = '';
	 	tr_view.style.display = '';
	 	
	 }
	 
	 function escondeAgrupador(id){
	 	var img_mais = document.getElementById('img_mais_' +id);
	 	var img_menos = document.getElementById('img_menos_' +id);
	 	var tr_view = document.getElementById('tr_view_' +id);
	 	
	 	img_mais.style.display = '';
	 	img_menos.style.display = 'none';
	 	tr_view.style.display = 'none';
	 	
	 }
	 
	 			</script>";
	 
	 echo $tabela;
}

if ( $_REQUEST["requisicao"] == 'cadastra' ){	
	$entid = pdeescola_pega_escola_atribuida($_SESSION["usucpf"]);

	if ( $entid ){
			
		if ( !pdeescola_possui_perfil(PDEESC_PERFIL_CAD_MAIS_EDUCACAO) ){
			
			// insere o usuário no perfil
			$sql = "insert into seguranca.perfilusuario (usucpf, pflcod) values
					('{$_SESSION["usucpf"]}',  " . PDEESC_PERFIL_CAD_MAIS_EDUCACAO. ")";	
			$db->executar($sql);
			$db->commit();
				
		}
		
		if ( !pdeescola_possui_escola_atribuida_cadastrador($_SESSION["usucpf"]) ){ 
			// cria a responsabilidade
			$sql = "insert into
						pdeescola.usuarioresponsabilidade 
						   (rpustatus,
						   	rpudata_inc,					   		
						   	entid,
						   	usucpf,
						   	pflcod 	
						   	)
						   	values
						   	('A',
						   	 now(),					   		
							 {$entid},
							 '{$_SESSION["usucpf"]}',
						   	 " . PDEESC_PERFIL_CAD_MAIS_EDUCACAO . ")";					   						 
		
			$db->executar($sql);
			$db->commit();
		}
		if( pdeescola_possui_perfil($arPerfilMaisEducacaoVeLista)) {
		
			?>
			<script>
				window.location.href = '/pdeescola/pdeescola.php?modulo=melista&acao=E';
			</script>							  	
			<?php
		} else {
			?>
			<script>
				redirecionaME('meajax.php', 'tipo=redirecioname&entid='+<?php echo $entid;?>);
			</script>							  	
			<?php
		}
	}else{ 
		$ano = $_SESSION["exercicio"];
		$anoAnterior = $ano -1;

			if ((($_GET['modalidade']) == 'M' && $_GET['memanoreferencia'] == $ano)) {?>
			    <script>
					window.location.href="/pdeescola/pdeescola.php?modulo=melista&acao=E&requisicao=cadastra&modalidade=M&memanoreferencia=<?php echo $ano ?>";
				</script><?php 
			}elseif(( ($_GET['modalidade']) == 'F' && $_GET['memanoreferencia'] == $ano)){ ?>
				<script>
					window.location.href="/pdeescola/pdeescola.php?modulo=melista&acao=E&requisicao=cadastra&modalidade=F&memanoreferencia=<?php echo $ano ?>";
				</script><?php 
			}elseif((($_GET['modalidade']) == 'M' && $_GET['memanoreferencia'] == $anoAnterior)){ ?>
				<script>
					window.location.href="/pdeescola/pdeescola.php?modulo=melista&acao=E&requisicao=cadastra&modalidade=M&memanoreferencia=<?php echo $anoAnterior ?>";
				</script><?php
			}elseif((($_GET['modalidade']) == 'F'&& $_GET['memanoreferencia'] == $anoAnterior) ){ ?>
				<script>
					window.location.href="/pdeescola/pdeescola.php?modulo=melista&acao=E&requisicao=cadastra&modalidade=F&memanoreferencia=<?php echo $anoAnterior ?>";
				</script><?php 				 		
			}else{?>
				<script>				
					window.location.href="/pdeescola/pdeescola.php?modulo=melista&acao=E&requisicao=cadastra";
				</script><?php
			}					
		}
}

//Monta o cabeçalho e título da tela
include  APPRAIZ."includes/cabecalho.inc";
echo"<br>";
monta_titulo("Painel", "Mais Educação");

$ano = $_SESSION["exercicio"];
$anoAnterior = $ano -1;

?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>

jQuery.noConflict();

jQuery(document).ready(function() {
	jQuery('.carrega').click(function(){
		var idDiv = jQuery(this).attr('id');
		var dados = jQuery('#param_'+idDiv).val();
		if( jQuery('#linha_'+idDiv).html() == '' ){
			jQuery('#linha_'+idDiv).html('Carregando...');
			jQuery.ajax({
		   		type: "POST",
		   		url: 'pdeescola.php?modulo=painel_maiseducacao&acao=A',
		   		data: "reqJson=montaPainel&dados="+dados,
		   		async: false,
		   		success: function(retorno){
					jQuery('#linha_'+idDiv).html(retorno);
					jQuery('#linha_'+idDiv).slideDown("slow");
		   		}
		 	});
		}else{
			if( jQuery('#linha_'+idDiv).css('display') == 'none' ){
				jQuery('#linha_'+idDiv).slideDown("fast");
				jQuery('#linha_'+idDiv).slideUp();
			}
		}
	});
});
</script>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<th>
			<a style="cursor:pointer;" onclick="window.location.href='/pdeescola/pdeescola.php?modulo=painel_maiseducacao&acao=A&requisicao=cadastra&modalidade=F&memanoreferencia=<?php echo $_SESSION["exercicio"]?>'">Mais Educação ( Ensino Fundamental - Ano <?php echo $_SESSION["exercicio"];?>)</a>
		</th>
	</tr>
	<tr>
		<th>
			<div id="div_1" class="carrega" style="cursor:pointer">
				<?php 
				/*
				    $ano = $_SESSION["exercicio"];
					$anoAnterior = $ano -1;
					$dados['anoRel'] = 2012;
					$dados['ano'] = $ano;
					$dados['anoAnterior'] = $anoAnterior;
					$dados['tipo'] = 'U';
					$dados['nomeTabela'] = 'tabela_2U';
					$qtd = pegaQtdPorAnoETipo($dados);
				*/
				?>
				<!-- 
				<input type="hidden" id="param_div_1" value="<?//=(str_replace('"','*',simec_json_encode($dados))) ?>" />
				ESCOLAS URBANAS (Total: <?//=$qtd ?>)
				 -->
			</div>
			<?
				//ano atual
				$ano = $_SESSION["exercicio"];
				$anoAnterior = $ano - 1;
				$dados['anoRel'] = $ano;
				$dados['ano'] = $ano;
				$dados['anoAnterior'] = $anoAnterior;
				$dados['nomeTabela'] = 'tabela_1';
				montaPainel( $dados );
			?>
		</th>
	</tr>
	<!-- 
	<tr><td valign="top" id="linha_div_1" style="display:none"></td></tr>
	<tr>
		<th>
			<div id="div_2" class="carrega" style="cursor:pointer">
				<?php 
					/*
					$dados['tipo'] = 'R';
					$dados['nomeTabela'] = 'tabela_2R';
					$qtd = pegaQtdPorAnoETipo($dados);
					*/
				?>
				<input type="hidden" id="param_div_2" value="<?//=(str_replace('"','*',simec_json_encode($dados))) ?>" />
				ESCOLAS RURAIS (Total: <?//=$qtd ?>)
			</div>
		</th>
	</tr>
	<tr><td valign="top" id="linha_div_2" style="display:none"></td></tr>
	<tr>
		<th>
			<div id="div_3" class="carrega" style="cursor:pointer">
				<?php 
					/*
					$dados['tipo'] = 'A';
					$dados['nomeTabela'] = 'tabela_2A';
					$qtd = pegaQtdPorAnoETipo($dados);
					*/
				?>
				<input type="hidden" id="param_div_3" value="<?//=(str_replace('"','*',simec_json_encode($dados))) ?>" />
				ESCOLAS ABERTAS (Total: <?//=$qtd ?>)
			</div>
		</th>
	</tr>
	 -->
	<tr><td valign="top" id="linha_div_3" style="display:none"></td></tr>
	<tr>
		<th>
			<a style="cursor:pointer;" onclick="window.location.href='/pdeescola/pdeescola.php?modulo=painel_maiseducacao&acao=A&requisicao=cadastra&modalidade=F&memanoreferencia=<?php echo $anoAnterior;?>'">Mais Educação ( Ensino Fundamental - Ano <?php echo $anoAnterior;?>)</a>
		</th>
	</tr>
	<tr>
		<th>
			<div id="div_4" class="carrega" style="cursor:pointer">
				<?php 
				/*
					$dados['anoRel'] = 2011;
					$dados['tipo'] = 'U';
				    $dados['nomeTabela'] = 'tabela_6U';
					$qtd = pegaQtdPorAnoETipo($dados);
				*/
				?>
				<!--  
				<input type="hidden" id="param_div_4" value="<?//=(str_replace('"','*',simec_json_encode($dados))) ?>" />
				ESCOLAS URBANAS (Total: <?//=$qtd ?>)
				-->
			</div>
			<?
				//$dados['anoRel'] = $anoAnterior;
				//$dados['nomeTabela'] = 'tabela_2';
				//montaPainel( $dados );
				
				//ano anterior
				$ano = ($_SESSION["exercicio"] - 1);
				$anoAnterior = $ano - 1;
				$dados['anoRel'] = $ano;
				$dados['ano'] = $_SESSION["exercicio"];
				$dados['anoAnterior'] = $ano;
				$dados['nomeTabela'] = 'tabela_2';
				montaPainel( $dados );
			?>
		</th>
	</tr>
	<tr>
		<th align="center">
			<table width="95%" align="center" border="0" cellspacing="0" cellpadding="0" >
				<tr bgcolor="DCDCDC" onmouseover="this.bgColor='#ffffcc'" onmouseout="this.bgColor='DCDCDC'">
					<td>
						<b>TOTAL GERAL:</b>
					</td>
					<td align="right" width="17%">
						<b>
							<?
							$sql = "SELECT  
										count(memid) as total
									FROM 
										pdeescola.memaiseducacao me
									INNER JOIN entidade.endereco ent ON ent.entid = me.entid
									WHERE
										me.memanoreferencia = ".$_SESSION['exercicio']." 
										AND me.memmodalidadeensino = 'F'
										AND me.memstatus = 'A'";
							$totalGeral = $db->pegaUm($sql);
							echo number_format($totalGeral,0,",",".");
							?>
						</b>
					</td>
				</tr>
			</table>
		</th>
	</tr>
	<!-- 
	<tr><td valign="top" id="linha_div_4" style="display:none"></td></tr>
	<tr>
		<th>
			<div id="div_5" class="carrega" style="cursor:pointer">
				<?php 
				/*
					$dados['tipo'] = 'R';
				    $dados['nomeTabela'] = 'tabela_6R';
					$qtd = pegaQtdPorAnoETipo($dados);
				*/
				?>
				<input type="hidden" id="param_div_5" value="<?//=(str_replace('"','*',simec_json_encode($dados))) ?>" />
				ESCOLAS RURAIS (Total: <?//=$qtd ?>)
			</div>
		</th>
	</tr>
	<tr><td valign="top" id="linha_div_5" style="display:none"></td></tr>
	<tr>
		<th>
			<div id="div_6" class="carrega" style="cursor:pointer">
				<?php 
				/*
					$dados['tipo'] = 'A';
				    $dados['nomeTabela'] = 'tabela_6A';
					$qtd = pegaQtdPorAnoETipo($dados);
				*/
				?>
				<input type="hidden" id="param_div_6" value="<?//=(str_replace('"','*',simec_json_encode($dados))) ?>" />
				ESCOLAS ABERTAS (Total: <?//=$qtd ?>)
			</div>
		</th>
	</tr>
	 -->
	<tr><td valign="top" id="linha_div_6" style="display:none"></td></tr>
</table>