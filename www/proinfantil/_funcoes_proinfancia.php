<?php
function listarObrasProInfantil() {
	global $db;
	
	$perfis = pegaPerfil($_SESSION["usucpf"]);
	
	if($_POST['obrnome']){
		$arrWhere[] = "removeacento(oi.obrnome) ilike removeacento(('%{$_POST['obrnome']}%'))";	
	}
	if($_POST['tpoid']){
		$arrWhere[] = "tpl.tpoid = {$_POST['tpoid']}";	
	}
	if($_POST['esdid']){
		$arrWhere[] = "edoc.esdid = {$_POST['esdid']}";	
	}
	if($_POST['stoid']){
		$arrWhere[] = "esd1.esdid = {$_POST['stoid']}";	
	}
	if($_POST['estuf']){
		$arrWhere[] = "tm.estuf = '{$_POST['estuf']}'";	
	}
	if($_POST['muncod']){
		$arrWhere[] = "tm.muncod = '{$_POST['muncod']}'";	
	}
	if($_POST['ano_atend']){
		$arrWhere[] = "SUBSTRING(trim(dt.resdsc), 7, 4) = '{$_POST['ano_atend']}'";	
	}
	
	if(!empty($_POST['analise1'])){
		if(in_array('S', $_POST['analise1']))
			$arrWhere[] = "(pinpareceraprovacao is not null and pinanoseguinte is not null and pinperiodorepasse is not null)";
		if(in_array('N', $_POST['analise1']))
			$arrWhere[] = "(pinpareceraprovacao is null or pinanoseguinte is null or pinperiodorepasse is null)";
	}
	
	if(!empty($_POST['analise2'])){
		if(in_array('S', $_POST['analise2']))
			$arrWhere[] = "(pinpareceraprovacao2 is not null and pinperiodorepasse2 is not null)";
		if(in_array('N', $_POST['analise2']))
			$arrWhere[] = "(pinpareceraprovacao2 is null or pinperiodorepasse2 is null)";
	}
	
	$arrWhere[] = "tpl.tpoid in (".OBRA_TIPO_A.",".OBRA_TIPO_B.",".OBRA_TIPO_C.",".MI_OBRA_TIPO_B.",".MI_OBRA_TIPO_C.")";	
	
	$sql = "SELECT			muncod 
			FROM 			proinfantil.usuarioresponsabilidade 
			WHERE			usucpf = '{$_SESSION['usucpf']}' 
			AND 			rpustatus = 'A'";
	$arrMunicipios = $db->carregarColuna($sql);
	if($arrMunicipios){
		$arrWhere[] = "tm.muncod in ('".implode("', '",$arrMunicipios)."')";
	}
	
	if(!in_array(EQUIPE_MUNICIPAL,$perfis)){
		$select = "CASE WHEN pinpareceraprovacao is not null OR pinanoseguinte is not null and pinperiodorepasse is not null then '<img src=\"../imagens/0_ativo.png\" title=\"Analisado\" alt=\"Analisado\"/>' else '<img src=\"../imagens/0_inativo.png\" alt=\"Não analisado\" title=\"Não analisado\"/>' end as analise1,
		           CASE WHEN  pinanoseguinte = 'N' OR ((SELECT to_date(r.resdsc, 'DD/MM/YYYY') FROM proinfantil.questionario q INNER JOIN questionario.resposta r ON r.qrpid = q.qrpid WHERE r.perid = 1587 AND q.pinid = pi.pinid) <= '2011-10-31') THEN '<b>_</b>'
		       	   WHEN pinpareceraprovacao2 IS NOT NULL AND pinperiodorepasse2 is not null then '<img src=\"../imagens/0_ativo.png\" title=\"Analisado\" alt=\"Analisado\"/>' else '<img src=\"../imagens/0_inativo.png\" alt=\"Não analisado\" title=\"Não analisado\"/>' end as analise2,";
		$cabecalho = array("Ação","Instituição","Nome da Obra","Data de Início","Data de Término","UF","Município","Tipo de Ensino","Situação da Obra","Percentual Executado (%)","Programa Fonte","Tipo da Obra","Tipologia","Situação do Plano","Análise 1","Análise 2","Data de Início de Atendimento");	
	} else {
		$select = "";
		$cabecalho = array("Ação","Instituição","Nome da Obra","Data de Início","Data de Término","UF","Município","Tipo de Ensino","Situação da Obra","Percentual Executado (%)","Programa Fonte","Tipo da Obra","Tipologia","Situação do Plano","Data de Início de Atendimento");	
	}	
	
	$sql = "SELECT
				CASE WHEN oi.obrpercentultvistoria >= 10
					THEN '<div style=\"white-space: nowrap\" ><img class=\"link\" src=\"../imagens/alterar.gif\" onclick=\"editarProInfantil(\'' || oi.obrid || '\')\" /></div>'
					ELSE '<div style=\"white-space: nowrap\" ><img class=\"link\" src=\"../imagens/alterar_01.gif\" onclick=\"alert(\'Operação não disponível!\')\" /></div>'
				END as acao,
		        upper(ee.entnome) as Nome_Instituicao,
		        upper(oi.obrnome) as Nome_Da_Obra,
		        to_char(oi.obrdtinicio,'dd-mm-yyyy') as DataInicio,
		        to_char(oi.obrdtfim,'dd-mm-yyyy')as DataTermino,
		        tm.estuf as UF,
		        tm.mundescricao as municipio,
				oo.orgdesc as Tipo_Ensino,
		        --st.stodesc as SituacaoObra,
		        esd1.esddsc as SituacaoObra,
		        oi.obrpercentultvistoria as Percentual_Executado,
		        pf.prfdesc as ProgramaFonte,
		        tp.tobdesc as TipodeObra,
		        COALESCE(tpl.tpodsc,'N/A') as TipologiaObra,
		        edoc.esddsc as descricaowork,
				$select
				CASE WHEN (dt.resdsc IS NULL OR  dt.resdsc = '') THEN 'Não Iniciado' ELSE dt.resdsc END
		    FROM
				obras2.obras AS oi
                INNER JOIN obras2.empreendimento e ON e.empid =  oi.empid
				INNER JOIN entidade.entidade AS ee ON oi.entid= ee.entid
				INNER JOIN entidade.endereco AS ed ON oi.endid = ed.endid
                INNER JOIN territorios.municipio AS tm ON ed.muncod = tm.muncod
                INNER JOIN obras2.orgao AS oo ON e.orgid = oo.orgid AND
                								 oo.orgstatus = 'A'                                                 
                INNER JOIN workflow.documento d1 ON d1.docid = oi.docid
                INNER JOIN workflow.estadodocumento esd1 on esd1.esdid = d1.esdid
                INNER JOIN obras2.programafonte AS pf ON e.prfid = pf.prfid
                INNER JOIN obras2.tipoobra AS tp ON oi.tobid = tp.tobid
                LEFT JOIN obras2.tipologiaobra AS tpl ON oi.tpoid = tpl.tpoid AND tpl.tpostatus = 'A'
                LEFT JOIN proinfantil.proinfantil pi ON pi.obrid = oi.obrid
                LEFT JOIN (SELECT 		q.pinid, r.resdsc
                           FROM 		proinfantil.questionario q
                           INNER JOIN 	questionario.resposta r on r.qrpid = q.qrpid
                           WHERE	 	r.perid = 1587 ) as dt ON dt.pinid = pi.pinid				
                LEFT JOIN workflow.documento d ON d.docid = pi.docid
                LEFT JOIN workflow.estadodocumento edoc on edoc.esdid = d.esdid
			WHERE
			    oi.obrstatus = 'A' AND
			    ee.entstatus = 'A' AND
			    pf.prfid = 41 AND
			    oi.obrpercentultvistoria >= 90 AND
                esd1.esdid IN (690, 693) and
			    oi.obridpai IS NULL
			    ".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
			ORDER BY
			    ee.entnome";
	//ver(simec_htmlentities($sql),d);
	$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', 'center', '' );
}

function cabecalhoProInfantil($obrid){
	
	global $db;
	
	$sql = "SELECT		muncod 
			FROM 		proinfantil.usuarioresponsabilidade 
			WHERE		usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A'";
	$arrMunicipios = $db->carregarColuna($sql);
	if($arrMunicipios){
		//$arrWhere[] = "tm.muncod in ('".implode("'",$arrMunicipios)."')";	
	}
	
	$sql = "SELECT
				upper(ee.entnome) as entnome,
			    upper(oi.obrnome) as obrnome,
			    tm.mundescricao,
			    tm.estuf,
			    COALESCE(tpl.tpodsc,'N/A') as tpodsc,
			    oi.obrpercentultvistoria as percentual_executado,
			    esd.esddsc as situacao_obra
			FROM
				obras2.obras oi
			    INNER JOIN workflow.documento 		doc ON doc.docid = oi.docid
			    INNER JOIN workflow.estadodocumento esd on esd.esdid = doc.esdid
			    INNER JOIN entidade.entidade 		ee ON oi.entid = ee.entid
			    INNER JOIN entidade.endereco 		ed ON oi.endid = ed.endid
			    INNER JOIN territorios.municipio 	tm ON ed.muncod = tm.muncod
			    LEFT  JOIN obras2.tipologiaobra  	tpl ON oi.tpoid = tpl.tpoid
			WHERE
				oi.obrid = {$obrid}
			".($arrWhere ? " and ".implode(" and ",$arrWhere) : "");
	
	$arrDados = $db->pegaLinha($sql);
	if($arrDados){
		extract($arrDados);
	} else {
		echo "<script>alert('Você não possui permissão para alterar informações desta obra!');window.location.href='proinfantil.php?modulo=principal/listaObrasMunicipio&acao=A'</script>";
		exit;
	}?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
	        <td width="25%" class="SubTituloDireita">Nome da Instituição:</td>
	        <td>
	        	<?php echo $entnome ?>
	        </td>
	    </tr>
		<tr>
	        <td class="SubTituloDireita">Nome da Obra:</td>
	        <td>
	        	<?php echo $obrnome ?>
	        </td>
	    </tr>
	    <tr>
	        <td class="SubTituloDireita">Município / UF:</td>
	        <td>
	        	<?php echo $mundescricao ?> / <?php echo $estuf ?>
	        </td>
	    </tr>
	    <tr>
	        <td class="SubTituloDireita">Tipologia:</td>
	        <td>
	        	<?php echo $tpodsc ?>
	        </td>
	    </tr>
	    <tr>
	        <td class="SubTituloDireita">Percentual Executado (%):</td>
	        <td><?php echo $percentual_executado; ?></td>
	    </tr>
	    <tr>
	        <td class="SubTituloDireita">Situação da Obra:</td>
	        <td><?php echo $situacao_obra; ?></td>
	    </tr>	
	    <?php 
	    $docid = recuperaDocumentoProInfantil($_SESSION['proinfantil']['pinid']);
	    $esdid = pegaEstadoAtualDocumento($docid);
	    ?>
	    <?php if( $esdid == WF_PROINFANTIL_EM_DILIGENCIA ){?>
	    <?php 
        	$sql = "SELECT 
						cmddsc
					FROM 
						workflow.comentariodocumento 
					WHERE
						hstid = (
							SELECT
								max(hstid)
							FROM
								workflow.historicodocumento
							WHERE
								docid = $docid
						)";
        	$cmddsc = $db->pegaUm($sql);
        	
        	if( $cmddsc == ''){
        		$cmddsc = 'Não informado';
        	}
        	
        	/*$sql = "SELECT		
					    --(cast(now() as date) - cast(hst.htddata as date)) as dias
					    
					FROM
					    proinfantil.proinfantil pi
					    inner join workflow.documento 		doc ON doc.docid = pi.docid
					    inner join workflow.estadodocumento esd on esd.esdid = doc.esdid
					    inner join workflow.historicodocumento hst on hst.docid = doc.docid and hst.aedid = 1033
					    inner join workflow.acaoestadodoc ae on ae.aedid = hst.aedid
					WHERE
					    pi.obrid = $obrid
					    and hst.hstid = (select max(h.hstid) from workflow.historicodocumento h where h.docid = doc.docid)";*/
        	
        	$sql = "SELECT
						hst.hstid,
					    hst.htddata,
					    hst.docid,
					    hst.aedid
					FROM
					    proinfantil.proinfantil pi
					    inner join workflow.documento 		doc ON doc.docid = pi.docid
					    inner join workflow.estadodocumento esd on esd.esdid = doc.esdid
					    inner join workflow.historicodocumento hst on hst.docid = doc.docid 
					WHERE
					    pi.obrid = $obrid
					  order by hst.htddata asc";        	
        	$arrWork = $db->carregar( $sql );
        	$arrWork = $arrWork ? $arrWork : array();
        	
        	$dias = calculaDiasVigencia($arrWork, AEDID_PRO_ENCAMINHAR_DILIGENCIA, AEDID_PRO_DILIGENCIA_ENVIAR_ANALISE);
        	
        	$diasDiligencia = (90 - (int)$dias);
    		$diasDiligencia = ((int)$diasDiligencia < 0 ? 0 : $diasDiligencia);
        ?>
	    <tr>
	        <td class="SubTituloDireita">Diligência:</td>
	        <td style="color:red;"><b><?=$cmddsc; ?></b></td>
	    </tr>
	    <tr>
	        <td class="subtituloCentro" colspan="2" style="color: red; ">Dias restantes para responder a diligência: <?=$diasDiligencia; ?> dias</td>
	    </tr>
	   	<?php }?> 
	    <?php if( $esdid == WF_PROINFANTIL_OBRA_ARQUIVADA || $esdid == WF_PROINFANTIL_OBRA_ARQUIVADA_SISTEMA ){?>
	    <?php 
        	$sql_parecer = "SELECT 	prapareceraprovacao
							FROM 	proinfantil.proinfanciaanalise 
							WHERE 	pinid = {$_SESSION['proinfantil']['pinid']} AND prastatus = 'A' AND praanoanalise = 1";

			$prapareceraprovacao = $db->pegaUm($sql_parecer);
        ?>
	    <tr>
	        <td class="SubTituloDireita">Indeferido:</td>
	        <td style="text-align:justify;color:red;"><b><?=$prapareceraprovacao; ?></b></td>
	    </tr>
	   	<?php }?>    
	</table> <?php
}

function verificaLiberacao(){
	$perfil = pegaPerfil($_SESSION['usucpf']);
	$estadoAtual = wf_pegarEstadoAtual( recuperaDocumentoProInfantil( $_SESSION['proinfantil']['pinid'] ) );
	
	if( in_array(PERFIL_SUPER_USUARIO,$perfil) || in_array(PERFIL_ADMINISTRADOR,$perfil)){
		return 'S';
	}

	if(in_array(EQUIPE_MUNICIPAL,$perfil)){
		$arrEsdid = Array(WF_PROINFANTIL_EM_CADASTRAMENTO, WF_PROINFANTIL_EM_DILIGENCIA);
		if( in_array($estadoAtual['esdid'],$arrEsdid) ){
			return 'S';
		} 
	}

	if(in_array(PERFIL_ANALISTA,$perfil)){
		$arrEsdid = Array(WF_PROINFANTIL_EM_ANALISE, WF_PROINFANTIL_EM_DILIGENCIA);
		if( in_array($estadoAtual['esdid'],$arrEsdid) ){
			return 'S';
		} 		
	}	
	
	if(in_array(PERFIL_COORDENADOR,$perfil)){
		$arrEsdid = Array(WF_PROINFANTIL_EM_ANALISE, WF_PROINFANTIL_EM_DILIGENCIA);
		if( in_array($estadoAtual['esdid'],$arrEsdid) ){
			return 'S';
		} 
	}	
	return 'N';
}

function recuperaDataEnvioAnalise($docid){
	global $db;
	if(!$docid){
		return false;
	} else {
		$sql = "SELECT   to_char(htddata,'DD/MM/YYYY') as data 
				FROM	 workflow.historicodocumento 
				WHERE 	 docid = {$docid} AND aedid = 1017 
				ORDER BY htddata ASC";
		return $db->pegaUm($sql);
	}
}

function recuperaDataInicioAtendimento($pinid){
	global $db;
	if(!$pinid){
		return false;
	} else {
   		$sql = "SELECT 		r.resdsc
				FROM 		proinfantil.questionario q
				INNER JOIN 	questionario.resposta r on r.qrpid = q.qrpid
				WHERE 		r.perid = 1587  and q.pinid = {$pinid}";
	    return $db->pegaUm($sql);
	}
}

function recuperaDocumentoProInfantil($pinid){
	global $db;
	if(!$pinid){
		return false;
	} else {
		$sql = "SELECT 	docid
				FROM	proinfantil.proinfantil
				WHERE	pinid = {$pinid} AND pinststus = 'A'";
		return $db->pegaUm($sql);
	}
}

function recuperaTipoTurno(){
	global $db;
	$sql = "SELECT * FROM proinfantil.tipoturno WHERE titstatus = 'A' AND modid = 1";
	return $db->carregar($sql);
}

function recuperaFotosSala($salid,$pinid){
	global $db;
	$sql = "SELECT			arq.arqid,
							arq.arqnome||'.'||arq.arqextensao,
							arq.arqdescricao,
							arq.arqtamanho
			FROM 			proinfantil.fotos fot
			INNER JOIN		public.arquivo arq ON arq.arqid = fot.arqid
			WHERE			fot.salid = {$salid}
			AND				fot.pinid = {$pinid}
			AND				fot.fotstatus = 'A'";
	return $db->carregar($sql);
}

function recuperaMaxAlunosPorObra($obrid){
	global $db;
	$sql = "SELECT		oi.tpoid
			FROM		obras2.obras AS oi
			LEFT JOIN   obras2.tipologiaobra AS tpl ON oi.tpoid = tpl.tpoid
			where		oi.obrid = {$obrid}";
	$tpoid = $db->pegaUm($sql);
	switch($tpoid){
		case OBRA_TIPO_A:
			return 500;
		break;
		case OBRA_TIPO_B:
		case MI_OBRA_TIPO_B:
			return 350;
		break;
		case OBRA_TIPO_C:
		case MI_OBRA_TIPO_C:
			return 180;
		break;
		default:
			return 0;
		break;
	}
}

function pegaPinid($obrid){
	global $db;
	$sql = "SELECT pinid FROM proinfantil.proinfantil WHERE obrid = {$obrid} AND pinststus = 'A'";
	$pinid = $db->pegaUm($sql);

	if(!$pinid){
		$docid = wf_cadastrarDocumento( WF_PROINFANTIL, "Alunos Atendidos - Proinfância");
		$vigid = recuperaVigenciaAtual();
		$usucpf = "'".$_SESSION['usucpf']."'";
		
		$sql = "INSERT INTO		proinfantil.proinfantil
								(obrid,docid,vigid,pindatainclusao,usucpf,pinststus)
				VALUES			($obrid,$docid,$vigid,now(),$usucpf,'A')
				RETURNING 		pinid";
		$pinid = $db->pegaUm($sql);
		$db->commit();
	}
	return $pinid;
}

function salvarDemandaProInfantil(){
	global $db;
	$obrid = $_GET['obrid'] ? $_GET['obrid'] : $_SESSION['proinfantil']['obrid'];
	extract($_POST);
		
	$max = recuperaMaxAlunosPorObra($obrid);
	if( $qtde_total_geral > $max ){
		$_SESSION['proinfantil']['mgs'] = "O número de alunos deve ser menor ou igual a $max!";
		header("Location: proinfantil.php?modulo=principal/demandaObraMunicipio&acao=A&obrid=$obrid");
		die;
	}
	
	$pinid = pegaPinid( $obrid );
	
	if($pinid){
		$sqlA = "DELETE FROM proinfantil.mdsalunoatendidopbf WHERE pinid = {$pinid};";
		if($qtde_mod_turno){
			foreach($qtde_mod_turno as $titid => $arrQtde){
				if($arrQtde){
					foreach($arrQtde as $timid => $qtde){
						$qtde = $qtde ? str_replace(".","",$qtde) : 0;
						$sqlA.="INSERT INTO proinfantil.mdsalunoatendidopbf (alaquantidade,pinid,timid,titid) VALUES ($qtde,$pinid,$timid,$titid);";
					}
				}
			}
		}
		
		$db->executar($sqlA);
		$db->commit();
		$_SESSION['proinfantil']['mgs'] = "Operação realizada com sucesso!";
		header("Location: proinfantil.php?modulo=principal/demandaObraMunicipio&acao=A&obrid=$obrid");
		die;
		
	}else{
		$_SESSION['proinfantil']['mgs'] = "Não foi possível realizar a operação!";
		header("Location: proinfantil.php?modulo=principal/demandaObraMunicipio&acao=A&obrid=$obrid");
		die;
	}
	
}

function recuperaSalas($obrid,$pinid){
	global $db;
	
	$aryModalidade = verificaExisteAluno($pinid);

	$where = '';
	
	if(in_array(CRECHE, $aryModalidade)){
		$where = 'AND sal.salid NOT IN (4,47,48,49,11,12,13,14)';
	}

	if(in_array(PREESCOLA, $aryModalidade)){
		$where = 'AND sal.salid NOT IN (1,2,3,8,9,10)';
	}
	
	if(in_array(CRECHE, $aryModalidade) && in_array(PREESCOLA, $aryModalidade)){
		$where = '';
	}	
		
	$sql = "SELECT 		DISTINCT sal.salid,
						tis.tisdescricao
			FROM		proinfantil.tiposala tis
			INNER JOIN	proinfantil.sala sal ON sal.tisid = tis.tisid
			WHERE		sal.tpoid IN(SELECT tpoid FROM obras2.obras WHERE obrid = {$obrid})
			$where
			ORDER BY	tis.tisdescricao";
	
	return $db->carregar($sql);
}

function salvarFotosSala(){
	global $db;
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	
	extract($_POST);
	$obrid = $_SESSION['proinfantil']['obrid'] ? $_SESSION['proinfantil']['obrid'] : $_REQUEST['obrid'];
	
	if($obrid){
		$pinid = recuperaProInfantil($obrid);
		
		if( $_FILES['arquivo']['tmp_name'] ){
			$arrCampos = array(
							"salid" => $salid,
							"pinid" => $pinid,
							"usucpf" => "'{$_SESSION['usucpf']}'",
							"fotstatus" => "'A'",
							"fotdatainclusao" => "now()"
						      );
			$file = new FilesSimec("fotos", $arrCampos, "proinfantil");
			$file->setUpload($arqdescricao, "arquivo");
			header("Location: proinfantil.php?modulo=principal/fotosObraMunicipio&acao=A");
			die;
		}else{
			$_SESSION['proinfantil']['mgs'] = "Não foi possível realizar a operação!";
			header("Location: proinfantil.php?modulo=principal/fotosObraMunicipio&acao=A");
			die;
		}
	}
	return false;	
}

function removerFotoSala(){
	global $db;
	$arqid = $_GET['arqid'];
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec("fotos",array(),"proinfantil");
	$file->excluiArquivoFisico($arqid);
	$sql = "delete from proinfantil.fotos where arqid = $arqid;";
	$sql.= "delete from public.arquivo where arqid = $arqid;";
	$db->executar($sql);
	$db->commit();
}


function geraTermoHtml($obrid, $pinid, $tipoano){
	global $db;
	
	$sql = "select
				upper(ee.entnome) as entnome,
		        upper(oi.obrnome) as obrnome,
				tm.mundescricao,
				tm.estuf,
				COALESCE(tpl.tpodsc,'N/A') as tpodsc,
				ee.entnumcpfcnpj as cnpj,
				ee.entcodent as codinep
			FROM
				obras2.obras AS oi
				INNER JOIN entidade.entidade AS ee ON oi.entid = ee.entid
				INNER JOIN entidade.endereco AS ed ON oi.endid = ed.endid
				INNER JOIN territorios.municipio AS tm ON ed.muncod = tm.muncod
				LEFT JOIN obras2.tipologiaobra AS tpl ON oi.tpoid = tpl.tpoid
			where
				oi.obrid = $obrid
			".($arrWhere ? " and ".implode(" and ",$arrWhere) : "");
	$arrDados = $db->pegaLinha($sql);
	if($arrDados){
		extract($arrDados);
	}
	
	if($tipoano == '1'){
		$sql = "select
						pinperiodorepasse, pinpareceraprovacao
					FROM
						proinfantil.proinfantil
					WHERE
						pinid = $pinid";
	}else{
		$sql = "select
						pinperiodorepasse2, pinpareceraprovacao2
					FROM
						proinfantil.proinfantil
					WHERE
						pinid = $pinid";
	}
	$dados = $db->pegaLinha($sql);
	if($dados) extract($dados);	
	
	
	$cnpj = $cnpj ? formatar_cnpj($cnpj) : "";
	
	
	$html = '<style type="text/css">
				.Tabela
				{
				    FONT-SIZE: xx-small;
				    FONT-FAMILY: Arial, Verdana;
				    BORDER-RIGHT: #cccccc 1px solid;
				    BORDER-TOP: #cccccc 1px solid;
				    BORDER-LEFT: #cccccc 1px solid;
				    BORDER-BOTTOM: #cccccc 1px solid;
					TEXT-DECORATION: none;
					WIDTH: 95%;
					TEXT-COLOR: #000000;
				}
				
				.SubTituloDireita
				{
				    FONT-SIZE: 8pt;
				    COLOR: black;
				    FONT-FAMILY: Arial, Verdana;
				    TEXT-ALIGN: right;
					BACKGROUND-COLOR: #dcdcdc;
				}
				.SubTituloEsquerda
				{
				    FONT-WEIGHT: bold;
				    FONT-SIZE: 8pt;
				    COLOR: black;
				    FONT-FAMILY: Arial, Verdana;
				    BACKGROUND-COLOR: #dcdcdc;
				    TEXT-ALIGN: left
				}
				
				.SubTituloCentro
				{
				    FONT-WEIGHT: bold;
				    FONT-SIZE: 8pt;
				    COLOR: black;
				    FONT-FAMILY: Arial, Verdana;
				    BACKGROUND-COLOR: #f0f0f0;
				    TEXT-ALIGN: center
				}
				
				.Label
				{
				    FONT-SIZE: 8pt;
				    COLOR: black;
				    FONT-FAMILY: Arial, Verdana;
				    TEXT-ALIGN: left
				}
				
			</style>';
	
	$html .= '<center>
				<img src="http://simec.mec.gov.br/imagens/brasao.gif" width="85" height="85" border="0">
				<BR>
				<font style="font-size: 14px;font-family: arial, sans-serif">
					<B>
						MINISTÉRIO DA EDUCAÇÃO
						<BR>
						SECRETÁRIA DE EDUCAÇÃO BÁSICA - SEB
						<br>
						DIRETORIA DE CURRÍCULOS E EDUCAÇÃO INTEGRAL - DICEI
						<BR>
						COORDENAÇÃO GERAL DE EDUCAÇÃO INFANTIL
					</B>
					<BR>
					Esplanada dos Ministérios, Bloco L, Edifício Sede, 5º Andar
					<br>
					CEP: 70.047-900 - Brasília, DF
			
					<BR><BR><BR><BR>
					
					<div align="left">
					<b>Análise Técnica - COEDI / DICEI / SEB / MEC</b>
					</div>
					
					<BR><BR>
				</font>
				
				<div align="right" style="font-size: 12px;font-family: arial, sans-serif">
					Brasília, '.date("d").' de '.mes_extenso2(date("m")).' de '.date("Y").'
				</div>
				
				<br><br>
				
				<font style="font-size: 14px;font-family: arial, sans-serif">
					<b>Identificação</b>
				</font>
				
				<table width="95%" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
					<tr>
				        <td width="25%" class="SubTituloDireita">CNPJ:</td>
				        <td class="Label">
				        	' . $cnpj . '
				        </td>
				    </tr>
					<tr>
				        <td width="25%" class="SubTituloDireita">Nome da Instituição:</td>
				        <td class="Label">
				        	' . $entnome . '
				        </td>
				    </tr>
					<tr>
				        <td class="SubTituloDireita">Nome da Obra:</td>
				        <td class="Label">
				        	' . $obrnome . '
				        </td>
				    </tr>
				    <tr>
				        <td class="SubTituloDireita">Município / UF:</td>
				        <td class="Label">
				        	' . $mundescricao . ' / ' . $estuf . '
				        </td>
				    </tr>
				    <tr>
				        <td class="SubTituloDireita">Tipologia:</td>
				        <td class="Label">
				        	' . $tpodsc . '
				        </td>
				    </tr>
				    <tr>
				        <td class="SubTituloDireita">Ano:</td>
				        <td class="Label">
				        	Ano '. ($tipoano == "2" ? "2" : "1") .'
				        </td>
				    </tr>';
	
	
	if($codigoinep){
		$html .= '	    <tr>
					        <td class="SubTituloDireita">Código INEP:</td>
					        <td class="Label">
					        	' . $codigoinep . '
					        </td>
					    </tr>';
	}
				    
	$html .= '	   <tr>
				        <td class="SubTituloDireita">Objeto da Análise:</td>
				        <td class="Label">
				        	Análise técnica das informações apresentadas pelo interessado para recebimento de recursos financeiros a título de apoio à manutenção de novo(s) estabelecimento(s) de educação infantil pública que esteja(m) em plena atividade e ainda não tenha(m) sido contemplado(s) com recursos do Fundeb.
				        </td>
				    </tr>
			
				</table>
				
				<br>';
	
	
	$arrModalidades = recuperaTipoModalidade();
	$arrAlunos = $pinid ? recuperaAlunoAtendido($pinid) : array();
	$periodoTotalMeses = $pinperiodorepasse;
		
		
	if($periodoTotalMeses >= 0){	
	
		$html .= '<table width="95%" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
						<tr >
					    	<td style="background-color: #C0C0C0;" colspan="5" class="SubTituloCentro">VALOR DO REPASSE</td>
					    </tr>				    
					    <tr >
					    	<td class="SubTituloEsquerda">Etapa</td>
					    	<td class="SubTituloEsquerda">Matrículas não Computadas <br>para Recebimento do Fundeb</td>
					    	<td class="SubTituloEsquerda">Valor Unitário</td>
					    	<td class="SubTituloEsquerda">Período Considerado para o Repasse<br> de Recursos (meses)</td>
					    	<td class="SubTituloEsquerda">Valor Total</td>
					    </tr>';
					    if($arrModalidades): 
	
					    	$somaColuna=0;
					    	$somaColuna4=0;
					    	$cor = "#FFFFFF";
					    	
					    	foreach($arrModalidades as $modalidade):
	
								$calculaLinha=0;
							    $cor = "#FFFFFF" ? "#f7f7f7" : "#FFFFFF";
							    $turno = 1;
							    $vlPeriodo = $db->pegaUm("select vaavalor as valor from proinfantil.valoraluno where vaastatus = 'A' and vaatipo = 'I' and timid = ".$modalidade['timid']);
							    $vlPeriodoMes = $vlPeriodo ? $vlPeriodo / 12 : 0;
							    
							    $somaColuna += $arrAlunos[$turno][$modalidade['timid']];
								$calculaLinha = $arrAlunos[$turno][$modalidade['timid']] ? $arrAlunos[$turno][$modalidade['timid']] : 0;
								
								$valorTotal = ($arrAlunos[$turno][$modalidade['timid']] * ($vlPeriodoMes ? $vlPeriodoMes : 0) * $periodoTotalMeses);
								$somaColuna4 += $valorTotal;
									        								
								$vlPeriodo = $vlPeriodo ? number_format($vlPeriodo,2,",",".") : "";
								$valorTotal = $valorTotal ? number_format($valorTotal,2,",",".") : "0,00";
								
								$html .= '<tr bgcolor="'.$cor.'" onmouseout="this.bgColor='.$cor.';" onmouseover="this.bgColor=#ffffcc;">
									        <td class="Label">'. $modalidade['timdescricao'] .' Integral</td>
									        <td class="Label">'. $calculaLinha .'</td>
									        <td class="Label">'. $vlPeriodo .'</td>
									        <td class="Label">'. ($periodoTotalMeses ? $periodoTotalMeses : "0") . ' / 12</td>
									        <td class="Label">'. $valorTotal .'</td>
									    </tr>';

								$calculaLinha=0;
								$cor = "#f7f7f7" ? "#FFFFFF" : "#f7f7f7";
								$turno++;
								
								$vlPeriodo = $db->pegaUm("select vaavalor as valor from proinfantil.valoraluno where vaastatus = 'A' and vaatipo = 'P' and timid = ".$modalidade['timid']);
							    $vlPeriodoMes = $vlPeriodo ? $vlPeriodo / 12 : 0;
								
								$somaColuna += $arrAlunos[$turno][$modalidade['timid']];
								$valorTotal = ($arrAlunos[$turno][$modalidade['timid']] * ($vlPeriodoMes ? $vlPeriodoMes : 0) * $periodoTotalMeses);
								$somaColuna4 += $valorTotal;
								
								$vlPeriodo = $vlPeriodo ? number_format($vlPeriodo,2,",",".") : "";
								$valorTotal = $valorTotal ? number_format($valorTotal,2,",",".") : "0,00";
							    
							    $html .= '<tr bgcolor="'.$cor.'" onmouseout="this.bgColor='.$cor.';" onmouseover="this.bgColor=#ffffcc;">
									        <td class="Label">'. $modalidade['timdescricao'] .' Parcial</td>
									        <td class="Label">'. $arrAlunos[$turno][$modalidade['timid']] .'</td>
									        <td class="Label">'. $vlPeriodo .'</td>
									        <td class="Label">'. ($periodoTotalMeses ? $periodoTotalMeses : "0") .' / 12</td>
									        <td class="Label">'. $valorTotal .'</td>
									    </tr>';
								
						    endforeach;
						    
						    $somaColuna4 = $somaColuna4 ? number_format($somaColuna4,2,",",".") : "0,00";
						    
						    $html .= '<tr >
								        <td class="SubTituloEsquerda" >Total Geral</td>
						        		<td class="SubTituloEsquerda" >'. $somaColuna .'</td>
								        <td class="SubTituloEsquerda" >&nbsp;</td>
								        <td class="SubTituloEsquerda" >&nbsp;</td>
								        <td class="SubTituloEsquerda" >'. $somaColuna4 .'</td>
								    </tr>';
					    endif;			
					    	    
					$html .= '</table>';
					
					$html .= '<BR>';
					
					$html .= '<table border="0" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
						<tr>
					    	<td align="left" style="font-size: 11px">
					    		(RESOLUÇÃO CD/FNDE Nº 52 DE 29 DE SETEMBRO DE 2011 - Art. 9º No ano de 2011, excepcionalmente,
					    		os estabelecimentos que iniciaram seu atendimento antes da publicação desta Resolução farão jus
					    		a, no máximo, 7/12 do valor aluno-ano definido pelo Fundeb para creche e pré-escola em período
					    		integral e parcial no exercício de 2010, conforme Portaria MEC 647, de 23 de maio de 2011.)
					    	</td>
					    </tr>
					</table>
					
					<BR>';
					
					
					$html .= '<table class="Tabela" border="0" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
						<tr>
					    	<td style="background-color: #C0C0C0;" class="SubTituloCentro">
					    		PARECER DE APROVAÇÃO
					    	</td>
					    </tr>
						<tr>
					    	<td class="Label">
					    		'.str_replace(chr(13),"<br>",$pinpareceraprovacao) .'
					    	</td>
					    </tr>				    
					</table>
					
					<BR>';
					
					$html .= '<table style="FONT-FAMILY: Arial, Verdana; FONT-SIZE: 8pt;" width="100%" cellSpacing="1" cellPadding="3" align="center">
						<tr>
					    	<td align="right" >
					    		<b>Usuário:</b> '.$_SESSION['usunome'].' 
					    		&nbsp;&nbsp;&nbsp;&nbsp;
					    		<b>Data:</b> '.date("d/m/Y H:i:s").'
					    	</td>
					    </tr>
					</table>';				
					
		
		} //fecha $periodoTotalMeses	
		
	$html .= '<center>';
	
	return $html;
}

function mes_extenso2($mes){
	if (strval($mes) == 1) return 'janeiro';
	else   if (strval($mes) == 2) return 'fevereiro';
	else   if (strval($mes) == 3) return 'março';
	else   if (strval($mes) == 4) return 'abril';
	else   if (strval($mes) == 5) return 'maio';
	else   if (strval($mes) == 6) return 'junho';
	else   if (strval($mes) == 7) return 'julho';
	else   if (strval($mes) == 8) return 'agosto';
	else   if (strval($mes) == 9) return 'detembro';
	else   if (strval($mes) == 10) return 'outubro';
	else   if (strval($mes) == 11) return 'novembro';
	else   if (strval($mes) == 12) return 'dezembro';
}


function verificaPagamentoEfetuado($docid){
	global $db;
	if(!$docid){
		return false;
	} else {
		$sql = "SELECT		hstid 
				FROM 		workflow.historicodocumento 
				WHERE 		docid = {$docid} AND aedid = 1034";
		return $db->pegaUm($sql);
	}	
}

function cancelaTermoProInfancia($dados){
	global $db;
	$dados['resposta'] = 'C';
	extract($dados);
	
	$sql_adpid = "SELECT 	adpid 
				  FROM		proinfantil.adesaoprograma 
				  where 	muncod = '{$_SESSION['proinfantil']['muncod']}' AND adpano = {date('Y')}";
	
	$adpid = $db->pegaUm($sql_adpid);
	
	$sql = "UPDATE 		proinfantil.adesaoprograma
			SET 		adpdataresposta = now(), 
					   	adpresposta = '{$resposta}', 
					    usucpf ='{$_SESSION['usucpf']}'
			WHERE 		adpid = {$adpid}";
	$db->executar($sql);
	
	//inserirProgramaHistorico($dados, $adpid);
	
	/*
	$sql = "DELETE FROM par.adesaoprograma 
			WHERE inuid = ".$_SESSION['par']['inuid']."
			AND pfaid = ".$_SESSION['par']['pfaid'];
	
	$db->executar($sql);
	*/
	$db->commit();
	$db->sucesso('principal/listaObrasMunicipio&acao=A');
}

function respondeTermoProInfancia($dados){
	global $db;
	extract($dados);

	if($resposta){
		$adesao = verificarAdesaoProInfancia($_SESSION['proinfantil']['pinid']);
		$data_adesao = date("Y-m-d G:i:s");

		if($adesao){
			$sql = "UPDATE 		proinfantil.proinfantil
   					SET			pinanoadesao = {$_SESSION['exercicio']},
   							    usucpfadesao = '{$_SESSION['usucpf']}',
   							    pindataadesao = '{$data_adesao}', 
   							    pinadesaopresposta = '{$resposta}'
 					WHERE 		pinid = {$_SESSION['proinfantil']['pinid']}";

			$db->executar($sql);
			$db->commit();
		} 
		
		if($resposta == 'S'){
			$db->sucesso('principal/questionarioMunicipio&acao=A');
		} else {
			echo '<script type="text/javascript"> 
		    		alert("Operação realizada com sucesso.");
		    		window.location.href="proinfantil.php?modulo=principal/listaObrasMunicipio&acao=A";
		    	  </script>';
		    exit();		
		}
	}
}

function verificaExisteAluno($pinid){
	global $db;
	$sql = "SELECT 			ma.timid
			FROM 			proinfantil.mdsalunoatendidopbf  ma
			INNER JOIN 		proinfantil.tipomodalidade tm ON tm.timid = ma.timid
			WHERE 			pinid = {$pinid} 
			GROUP BY 		ma.timid, tm.timdescricao 
			HAVING 			sum(ma.alaquantidade) > 0";
	$aryModalidade = $db->carregar($sql);
	
    !$aryModalidade? $aryModalidade = array() : $aryModalidade = $aryModalidade;
    $aryMod = array();
	foreach($aryModalidade as $mod){
	   	$aryMod[] = $mod['timid'];
    }

    return $aryMod;
}

function enviarparaanalise( $pinid ){
	global $db;
	$msg = true;
	
	/* $obrid = $_SESSION['proinfantil']['obrid'];
	
	$arrRetorno = recuperaSalas($obrid,$pinid);
	$arrRetorno = $arrRetorno ? $arrRetorno : array();
	
	foreach ($arrRetorno as $v) {
		$sql = "SELECT COUNT(arqid) as total FROM proinfantil.fotos WHERE salid = {$v['salid']} AND pinid = {$pinid} AND fotstatus = 'A'";
		$total = $db->pegaUm($sql);
		if($total == 0) $msg = "Na Aba Fotos do Estabelecimento de Ensino, é necessário adicionar pelo menos uma foto no ambiente:  {$v['tisdescricao']}!";
	} */
	
	$aryModalidade = verificaExisteAluno($pinid);
	
	$sql = "SELECT		COUNT(arq.arqid) as total
			FROM 		proinfantil.fotos fot
			INNER JOIN	public.arquivo arq ON arq.arqid = fot.arqid
			WHERE 		fot.salid IN ( select salid from proinfantil.sala where tisid in ( 23 ) order by salid ) AND fot.pinid = {$pinid} AND fot.fotstatus = 'A'";
	$total = $db->pegaUm($sql);
	if($total == 0) $msg = "Na Aba Fotos do Estabelecimento de Ensino, é necessário adicionar pelo menos uma foto no ambiente:  Pátio!";
	
	$sql = "SELECT		COUNT(arq.arqid) as total
			FROM 		proinfantil.fotos fot
			INNER JOIN	public.arquivo arq ON arq.arqid = fot.arqid
			WHERE 		fot.salid IN ( select salid from proinfantil.sala where tisid in ( 22 ) order by salid ) AND fot.pinid = {$pinid} AND fot.fotstatus = 'A'";
	$total = $db->pegaUm($sql);
	if($total == 0) $msg = "Na Aba Fotos do Estabelecimento de Ensino, é necessário adicionar pelo menos uma foto no ambiente:  Cantina e Refeitório!";
	
	$sql = "SELECT		COUNT(arq.arqid) as total
			FROM 		proinfantil.fotos fot
			INNER JOIN	public.arquivo arq ON arq.arqid = fot.arqid
			WHERE 		fot.salid IN ( select salid from proinfantil.sala where tisid in ( 21 ) order by salid ) AND fot.pinid = {$pinid} AND fot.fotstatus = 'A'";
	$total = $db->pegaUm($sql);
	if($total == 0) $msg = "Na Aba Fotos do Estabelecimento de Ensino, é necessário adicionar pelo menos uma foto no ambiente: Geral da Unidade!";
	
	if(in_array(CRECHE, $aryModalidade)){
		$sql = 
"SELECT COUNT(arq.arqid) AS total
FROM proinfantil.fotos fot
INNER JOIN public.arquivo arq ON arq.arqid = fot.arqid
WHERE fot.salid IN
    ( SELECT salid
     FROM proinfantil.sala
     WHERE tisid IN (1,
                     2,
                     3,
                     4,
                     5,
                     6,
                     7,
                     8,
                     9,
                     10)
     ORDER BY salid )
  AND fot.pinid = {$pinid}
  AND fot.fotstatus = 'A'

";
		$total = $db->pegaUm($sql);
		if($total == 0) $msg = "Na Aba Fotos do Estabelecimento de Ensino, é necessário adicionar pelo menos uma foto em um dos ambientes: Creche!";	
	}

	if(in_array(PREESCOLA, $aryModalidade)){
		$sql = "
SELECT COUNT(arq.arqid) AS total
FROM proinfantil.fotos fot
INNER JOIN public.arquivo arq ON arq.arqid = fot.arqid
WHERE fot.salid IN
    (SELECT salid
     FROM proinfantil.sala
     WHERE tisid IN (11,
                     12,
                     13,
                     14,
                     16,
                     17,
                     18,
                     15,
                     19,
                     20 )
     ORDER BY salid)
  AND fot.pinid = {$pinid}
  AND fot.fotstatus = 'A'

";
		$total = $db->pegaUm($sql);
		if($total == 0) $msg = "Na Aba Fotos do Estabelecimento de Ensino, é necessário adicionar pelo menos uma foto em um dos ambientes: Pré-Escola!";	
	}
	
	$sql = "SELECT 		COUNT(alaquantidade) 
			FROM 		proinfantil.mdsalunoatendidopbf 
			WHERE 		pinid = {$pinid}";
	$qtd = $db->pegaUm( $sql );
	if($qtd == 0) $msg = "Na Aba Crianças Atendidas, o total de Matrículas deve ser maior que zero!";
	
	$sql = "SELECT		COUNT(r.resid) 
			FROM 		questionario.resposta r
			INNER JOIN	proinfantil.questionario pq ON pq.qrpid = r.qrpid
			WHERE 		r.perid in (1587, 1588) AND pinid = {$pinid}";
	$qtdQuest = $db->pegaUm( $sql );
	if($qtdQuest == 0) $msg = "Na Aba Questionário, é necessário preencher as questões 1 e 2!";
	
	return $msg;
}

function enviarAnalisado( $pinid ){
	global $db;
	$msg = true;
	$sql_data = "SELECT to_date(r.resdsc, 'DD/MM/YYYY') AS data 
				FROM proinfantil.questionario q 
				INNER JOIN questionario.resposta r ON r.qrpid = q.qrpid 
				WHERE r.perid = 1587 AND q.pinid = {$pinid}";
	$rs_data = $db->pegaUm($sql_data);				
	
	$sql_parecer = "SELECT pinid 
					FROM proinfantil.proinfanciaanalise 
					WHERE pinid = {$pinid}
					AND prapareceraprovacao IS NOT NULL";
	$rs_parecer = $db->pegaUm($sql_parecer);
	
	if(empty($rs_data) || empty($rs_parecer)){
		$msg = "É necessário gerar o parecer da Aba Análise!";
	}
	
	return $msg;
}

function enviarValidacao($pinid = null){
	global $db;
	
	if(!$pinid) return false;
	
	$msg = true;
	
	$abaAnalise = verificarAbaAnalise($pinid);

	$sql_parecer = "SELECT pinid 
					FROM proinfantil.proinfanciaanalise 
					WHERE pinid = {$pinid}
					AND prapareceraprovacao IS NOT NULL";
	$rs_parecer = $db->pegaUm($sql_parecer);			
	
	if( $abaAnalise  == 'S'){
		$sql = "SELECT pinid 
				FROM proinfantil.proinfantil 
				WHERE pinid = {$pinid} 
				AND pinanoseguinte IS NOT NULL
				AND pinperiodorepasse IS NOT NULL";
		$rs = $db->pegaUm($sql);		

		$sql_data = "SELECT to_date(r.resdsc, 'DD/MM/YYYY') AS data 
					FROM proinfantil.questionario q 
					INNER JOIN questionario.resposta r ON r.qrpid = q.qrpid 
					WHERE r.perid = 1587 AND q.pinid = {$pinid} AND to_date(r.resdsc, 'DD/MM/YYYY') <= '2011-10-31'";
		$rs_data = $db->pegaUm($sql_data);				
		
		if((empty($rs) && empty($rs_data)) || empty($rs_parecer)){
			$msg = "É necessário gerar o parecer da Aba Análise Ano 1!";	
		}		
	}
	
	$sql = "SELECT
				COUNT(arq.arqid) AS total
			FROM
				proinfantil.fotos fot
			INNER JOIN
				public.arquivo arq ON arq.arqid = fot.arqid
			INNER JOIN
				proinfantil.sala sal ON sal.salid = fot.salid AND sal.tisid = 23					
			WHERE
				fot.pinid = {$pinid} AND fot.fotstatus = 'A'";
	$total = $db->pegaUm($sql);
	if($total == 0) {
		$msg = "Na Aba Fotos do Estabelecimento de Ensino, é necessário adicionar pelo menos uma foto no ambiente:  Pátio!";
	}
	
	$sql = "SELECT
				COUNT(arq.arqid) AS total
			FROM
				proinfantil.fotos fot
			INNER JOIN
				public.arquivo arq ON arq.arqid = fot.arqid
			INNER JOIN
				proinfantil.sala sal ON sal.salid = fot.salid AND sal.tisid = 22				
			WHERE 
				fot.pinid = {$pinid} AND fot.fotstatus = 'A'";
	$total = $db->pegaUm($sql);
	
	if($total == 0) {
		$msg = "Na Aba Fotos do Estabelecimento de Ensino, é necessário adicionar pelo menos uma foto no ambiente:  Cantina e Refeitório!";
	}
	
	$sql = "SELECT
				COUNT(arq.arqid) AS total
			FROM
				proinfantil.fotos fot
			INNER JOIN
				public.arquivo arq ON arq.arqid = fot.arqid
			INNER JOIN
				proinfantil.sala sal ON sal.salid = fot.salid AND sal.tisid = 21
			WHERE 
				fot.pinid = $pinid AND fot.fotstatus = 'A'";
	
	$total = $db->pegaUm($sql);
	if($total == 0) {
		$msg = "Na Aba Fotos do Estabelecimento de Ensino, é necessário adicionar pelo menos uma foto no ambiente: Geral da Unidade!";
	}
	
	$sql = "SELECT
				COUNT(arq.arqid) AS total
			FROM 
				proinfantil.fotos fot
			INNER JOIN
				public.arquivo arq ON arq.arqid = fot.arqid
			INNER JOIN
				proinfantil.sala sal ON sal.salid = fot.salid AND sal.tisid NOT IN (21,22,23)				
			WHERE
				fot.pinid = {$pinid} AND fot.fotstatus = 'A'";
	$total = $db->pegaUm($sql);
	if($total == 0) {
		$msg = "Na Aba Fotos do Estabelecimento de Ensino, é necessário adicionar pelo menos uma foto em um dos ambientes:  Creche ou Pré-Escola!";	
	}

	return $msg;
}

function enviarPagamentoEfetuado(){
	global $db;
	$perfil = pegaPerfil($_SESSION['usucpf']);

	if( in_array(PERFIL_SUPER_USUARIO, $perfil) || in_array(PERFIL_ANALISTA_PAGAMENTO, $perfil)){
		return true;
	} else {
		return "Seu perfil não tem acesso ao envio de pagamento!";
	}
}

function enviarPagamento(){
	global $db;
	
	$perfil = pegaPerfil($_SESSION['usucpf']);

	if( in_array(PERFIL_SUPER_USUARIO, $perfil) || in_array(PERFIL_COORDENADOR, $perfil) || in_array(PERFIL_ADMINISTRADOR, $perfil) ){
		return true;
	} else {
		return "Seu perfil não tem acesso ao envio de pagamento!";
	}
}

function verificarAbaAnalise($pinid){
	global $db;
	
	if( $pinid ){
		$sql = "SELECT trim(pi.pintipoabaanalise) FROM proinfantil.proinfantil pi WHERE pi.pinid = {$pinid}";	
		$rs = $db->pegaUm($sql);
	}
	return $rs;
}

function verificarQuestionario($pinid,$esdid){
	global $db;

	if($esdid == WF_PROINFANTIL_EM_CADASTRAMENTO){
		$sql_quest = "SELECT  			COUNT(perid) as qtd
					  FROM	       		proinfantil.questionario que
					  INNER JOIN     	questionario.questionarioresposta qr ON qr.qrpid = que.qrpid
					  INNER JOIN     	questionario.resposta re ON re.qrpid = qr.qrpid
					  WHERE          	que.pinid = {$pinid} 
					  AND				qr.queid = ".QUESTIONARIO;
		$rs_quest = $db->pegaLinha($sql_quest);
		
		if($rs_quest['qtd'] == 4){
			
			$sql = "SELECT  		perid, itpid, resdsc
					FROM	       	proinfantil.questionario que
					INNER JOIN     	questionario.questionarioresposta qr ON qr.qrpid = que.qrpid
					INNER JOIN     	questionario.resposta re ON re.qrpid = qr.qrpid
					WHERE          	que.pinid = {$pinid} 
					AND				qr.queid = ".QUESTIONARIO."
					ORDER BY 		perid";
			
			$resposta = $db->carregar($sql);
			
			foreach($resposta as $resp){
				
				if($resp['perid']=='3578'){
					$qrpid = pegaQrpid($pinid);
					
					$total = $db->pegaUm("SELECT count(qciid) FROM proinfantil.questionariocodigoinep WHERE perid = {$resp['perid']} and qrpid = $qrpid");
					
					if( (int)$total <> 0 ){
						return 'N';
					} else {
						return 'S';
					}
				}
								
				if($resp['perid']=='1587' || $resp['perid']=='3578'){
					if(empty($resp['resdsc'])){
						return 'S';
					}
				}
				
				if($resp['perid']=='1588' || $resp['perid']=='3579'){
					if(empty($resp['itpid'])){
						return 'S';
					}
				}			
			}
		} else {
			return 'S';
		}
		return 'N';
	} else {
		return 'N';
	}
}

function verificarRespostaRadio($perid,$pinid){
	global $db;
	
	$sql = "SELECT  		itpid
			FROM	       	proinfantil.questionario que
			INNER JOIN     	questionario.questionarioresposta qr ON qr.qrpid = que.qrpid
			INNER JOIN     	questionario.resposta re ON re.qrpid = qr.qrpid
			WHERE          	que.pinid = {$pinid} and perid = {$perid}
			AND				qr.queid = ".QUESTIONARIO."
			ORDER BY 		perid";

	$rs = $db->pegaUm($sql);
	return $rs;	
}

function listaMunicipiosSemLote( $post ){
	
	global $db;
	
	extract($post);
	
	$arrWhere = Array("edoc.esdid = ". WF_PROINFANTIL_ANALISADO);
	
	if($estuf){
		$arrWhere[] = "tm.estuf = '{$estuf}'";
	}
	
	if($mundescricao){
		$arrWhere[] = "tm.mundescricao ilike '%{$mundescricao}%'";
	}
	
	$arrWhere[] = "tpl.tpoid in (".OBRA_TIPO_A.",".OBRA_TIPO_B.",".OBRA_TIPO_C.")";	
	
	$sql = "select muncod from proinfantil.usuarioresponsabilidade where usucpf = '{$_SESSION['usucpf']}' and rpustatus = 'A'";
	$arrMunicipios = $db->carregarColuna($sql);
	
	if($arrMunicipios){
		$arrWhere[] = "tm.muncod in ('".implode("', '",$arrMunicipios)."')";
	}
	
	$sql = "SELECT
				'<center>
					<input type=\"checkbox\" name=\"pinid[]\" value=\"'|| pi.pinid ||'\" /> 
					<input type=\"hidden\" name=\"docid['|| pi.pinid ||']\" value=\"'|| pi.docid ||'\" />
				</center>' as pinid,
				upper(ee.entnome) as Nome_Instituicao,
		        upper(oi.obrnome) as Nome_Da_Obra,
		        tm.estuf as UF,
		        tm.mundescricao as municipio,
		        edoc.esddsc as descricaowork
		    FROM
				obras2.obras AS oi
				inner join obras2.empreendimento e ON e.empid =  oi.empid
				inner join entidade.entidade AS ee ON oi.entid = ee.entid
				inner join entidade.endereco AS ed ON oi.endid = ed.endid
				inner join territorios.municipio AS tm ON ed.muncod = tm.muncod
				inner join workflow.documento d1 ON d1.docid = oi.docid
                inner join workflow.estadodocumento esd1 on esd1.esdid = d1.esdid
                inner join obras2.programafonte AS pf ON e.prfid = pf.prfid
                inner join obras2.tipoobra AS tp ON oi.tobid = tp.tobid
				inner join proinfantil.proinfantil pi ON pi.obrid = oi.obrid
                LEFT JOIN obras2.tipologiaobra AS tpl ON oi.tpoid = tpl.tpoid AND tpl.tpostatus = 'A'
				left join workflow.documento d ON d.docid = pi.docid
				left join workflow.estadodocumento edoc on edoc.esdid = d.esdid
			WHERE
			    oi.obrstatus = 'A'
			    and ee.entstatus = 'A'
			    and pf.prfid = 41
			    and oi.obrpercentultvistoria >= 90
                and esd1.esdid IN (690, 693)
                and oi.obridpai IS NULL
			    and pi.lotid is null
			    ".($arrWhere ? " and ".implode(" and ",$arrWhere) : "")."
			ORDER BY
			    tm.mundescricao";
	
	$cabecalho = array('Ação &nbsp;<input type=\'checkbox\' class=\'todos\'>', "Instituição", "Nome da Obra", 'UF', 'Município', 'Situação');
	$db->monta_lista_simples($sql, $cabecalho, 6000, 1, '', '100%', '', '');
}


function listaMunicipiosComLote( $post ){
	global $db;
	
	extract($post);
	
	$perfil = pegaPerfil($_SESSION['usucpf']);
	
	$acaoSQLDisabled = " '<input type=\"checkbox\" disabled=\"disabled\" checked=\"checked\" name=\"docid[]\" value=\"'|| doc.docid ||'\" />' ";
	if( in_array(PERFIL_ANALISTA_PAGAMENTO, $perfil) || $db->testa_superuser() ){
            $acaoSQL = " '<input type=\"checkbox\" name=\"docid[]\" value=\"'|| doc.docid ||'\" />' ";
	} else {
            $acaoSQL = " '<input type=\"checkbox\" disabled=\"disabled\" checked=\"checked\" name=\"docid[]\" value=\"'|| doc.docid ||'\" />' ";
	}
	
	$arWhere = Array('1=1');
	if($estuf){
            $arWhere[] = "mun.estuf = '{$estuf}'";
	}
	
	if($mundescricao){
            $arWhere[] = "mun.mundescricao ilike '%{$mundescricao}%'";
	}
	
	if($lotid){
            $arWhere[] = "lot.lotid = $lotid";
	}
	
	$sql = "SELECT DISTINCT 
                    '<center>'||CASE WHEN esd.esdid = ".WF_PROINFANTIL_ENVIADO_PARA_PAGAMENTO." THEN $acaoSQL ELSE $acaoSQLDisabled END||'</center>' as acao,
                    iue.iuecnpj,
                    upper(ee.entnome) as Nome_Instituicao,
                    upper(oi.obrnome) as Nome_Da_Obra,
                    muni.estuf as UF,
                    muni.muncod,
                    muni.mundescricao as municipio,
                    lot.lotid,
                    (
                        SELECT
                            trim(to_char(lot.valorrepasse, '999G999G999G999G999D99')) as valorrepasse
                        FROM 
                            proinfantil.loteminutaproinfantil lot
                            inner join territorios.municipio mun on mun.muncod = lot.muncod
                            left join par.instrumentounidade iu
                        inner join par.instrumentounidadeentidade iue on iue.inuid = iu.inuid
                        on iu.muncod = mun.muncod
                        WHERE lot.lotid = {$lotid} and iu.muncod = muni.muncod
                    ) as valorMunicipio,
                    esd.esddsc as descricaowork
                FROM 
                    obras2.obras AS oi
                INNER JOIN entidade.entidade AS ee ON oi.entid = ee.entid
                INNER JOIN entidade.endereco AS ed ON oi.endid = ed.endid
                INNER JOIN territorios.municipio AS muni ON ed.muncod = muni.muncod
                INNER JOIN proinfantil.proinfantil 		dcm ON dcm.obrid = oi.obrid
                INNER JOIN workflow.documento			doc ON doc.docid = dcm.docid
                INNER JOIN workflow.estadodocumento		esd ON esd.esdid = doc.esdid 					   
                INNER JOIN proinfantil.loteproinfancia	lot ON lot.lotid = dcm.lotid
                LEFT JOIN par.instrumentounidade iu 
                INNER JOIN par.instrumentounidadeentidade iue on iue.inuid = iu.inuid on iu.muncod = muni.muncod
                WHERE
                    ".implode(' AND ', $arWhere)."
                ORDER BY 3,2";
        
        $arrDados = $db->carregar($sql);
        
        $arrRegistro = array();
	foreach ($arrDados as $key => $valor) {
            $arrRegistro[$key] = array(
                'acao'=> $valor['acao'],								
                'iuecnpj' => $valor['iuecnpj'],								
                'nome_instituicao' => $valor['nome_instituicao'],								
                'nome_da_obra' => $valor['nome_da_obra'],								
                'uf' => $valor['uf'],								
                'municipio' => $valor['municipio'],
                'muncod' => $valor['muncod'],
                'valormunicipio' => $valor['valormunicipio'],								
                'descricaowork' => $valor['descricaowork'],								
                'lotid' => $valor['lotid'],
                'rowspan' => countMunicipioObraPorLote($lotid, $valor['muncod'])
            );
	}
        ?>
        <table class="listagem" width="95%" border="1" align="center" cellspacing="0" cellpadding="2">
            <thead>
                <tr>
                    <td class="title" valign="top" bgcolor="" align="" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" 
                        style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Ação</td>
                    <td class="title" valign="top" bgcolor="" align="" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" 
                        style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">CNPJ</td>
                    <td class="title" valign="top" bgcolor="" align="" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" 
                        style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Instituição</td>
                    <td class="title" valign="top" bgcolor="" align="" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" 
                        style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Nome da Obra</td>
                    <td class="title" valign="top" bgcolor="" align="" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" 
                        style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">UF</td>
                    <td class="title" valign="top" bgcolor="" align="" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" 
                        style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Município</td>
                    <td class="title" valign="top" bgcolor="" align="" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" 
                        style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Valor Municípios</td>
                    <td class="title" valign="top" bgcolor="" align="" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#c0c0c0';" 
                        style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">Situação</td>
                </tr>
            </thead>
            <tbody>
                 <?php    
                if( $arrRegistro ){ 
                    $muncod = '';
                    foreach ($arrRegistro as $key => $valor) {
                    ?>
                        <tr id="tr_<?php echo $key; ?>" onmouseout="this.bgColor='';" onmouseover="this.bgColor='#ffffcc';">
                            <td align="left" title="Ação"><?php echo $valor['acao']; ?></td>
                            <td align="left" title="CNPJ"><?php echo $valor['iuecnpj']; ?></td>
                            <td align="left" title="Instituição" style="color:#0066cc;"><?php echo $valor['nome_instituicao']; ?></td>
                            <td align="left" title="Nome da Obra" style="color:#0066cc;"><?php echo $valor['nome_da_obra']; ?></td>
                            <td align="left" title="UF"><?php echo $valor['uf']; ?></td>
                            <td align="left" title="Município"><?php echo $valor['municipio']; ?></td>
                            <?php
                            if( $muncod != $valor['muncod'] ){
                            ?>
                                <td rowspan="<?php echo $valor['rowspan'] ?>" align="right" title="Valor Município" style="color:#0066cc;">R$ <?php echo $valor['valormunicipio']; ?></td>
                            <?php
                            }
                            ?>
                            <td align="left" title="Situação"><?php echo $valor['descricaowork']; ?></td>
                        </tr>
                <?php
                    $muncod = $valor['muncod'];
                    } ?>
            </tbody>
        </table>
        <table class="listagem" width="95%" border="0" align="center" cellspacing="0" cellpadding="2">
            <tbody>
                <tr bgcolor="#ffffff">
                    <td><b>Total de Registros: <?php echo sizeof($arrRegistro); ?></b></td>
                </tr>
            </tbody>
	</table>
        <?php }else{ ?>
        <table class="listagem" width="95%" border="0" align="center" cellspacing="0" cellpadding="2">
            <tbody>
                <tr>
                    <td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td>
                </tr>
            </tbody>
	</table>
        <?php
        } # fechamento do Else
        
	$sql = "SELECT DISTINCT
                    doc.esdid
                FROM 
                    proinfantil.loteproinfancia lot
                    INNER JOIN proinfantil.proinfantil 	dcm ON dcm.lotid = lot.lotid
                    LEFT  JOIN workflow.documento		doc ON doc.docid = dcm.docid AND doc.esdid = ".WF_PROINFANTIL_ENVIADO_PARA_PAGAMENTO."
                    LEFT  JOIN workflow.estadodocumento	esd ON esd.esdid = doc.esdid
                WHERE 
                    lotstatus = 'A' AND lot.lotid = $lotid
                ORDER BY 1";
	$testaLote = $db->pegaUm($sql);
	if( ( in_array(PERFIL_ANALISTA_PAGAMENTO, $perfil) || $db->testa_superuser()) && $testaLote != '' && $arrRegistro){
        ?>	
            <center>
		<input type="button" value="Confirmar Pagamento por Município" class="tramitaDocid"/>
            </center>
        <?php 
	}
}#Fim da funcao listaMunicipiosComLote

function countMunicipioObraPorLote($lotid = null, $muncod = null){
    
    global $db;    
   
    $sql = "SELECT
                count(iuecnpj)
            FROM 
                obras2.obras AS oi
            INNER JOIN entidade.entidade AS ee ON oi.entid = ee.entid
            INNER JOIN entidade.endereco AS ed ON oi.endid = ed.endid
            INNER JOIN territorios.municipio AS muni ON ed.muncod = muni.muncod
            INNER JOIN proinfantil.proinfantil 		dcm ON dcm.obrid = oi.obrid
            INNER JOIN workflow.documento			doc ON doc.docid = dcm.docid
            INNER JOIN workflow.estadodocumento		esd ON esd.esdid = doc.esdid 					   
            INNER JOIN proinfantil.loteproinfancia	lot ON lot.lotid = dcm.lotid
            LEFT JOIN par.instrumentounidade iu 
            INNER JOIN par.instrumentounidadeentidade iue on iue.inuid = iu.inuid on iu.muncod = muni.muncod
            WHERE 1=1 AND lot.lotid = {$lotid}  and muni.muncod = '{$muncod}'";
    
    return $db->pegaUm($sql);
}

function geraLote( $request ){	
	global $db;
	
	require_once APPRAIZ . 'includes/workflow.php';
	
	extract($request);
	
	if( $pinid[0] != '' ){
		$dataPortaria = explode('/', $lotdataportaria);
		$dia = $dataPortaria[0];
		$mes = $dataPortaria[1];
		$ano = $dataPortaria[2];
		$mes = mes_extenso($mes);
	
		$texto = '<p style="text-align: justify;"><strong>PORTARIA N&ordm;&nbsp;&nbsp;'.$lotnumportaria.', &nbsp;&nbsp;&nbsp;&nbsp;DE&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'.$dia.'&nbsp;&nbsp;&nbsp;&nbsp; DE &nbsp;&nbsp;&nbsp;&nbsp;'.$mes.'&nbsp;&nbsp;&nbsp;&nbsp;DE '.$ano.'.</strong></p>
<p style="text-align: justify;">&nbsp;</p>
<p style="padding-left: 300px; text-align: justify;">Autoriza o Fundo Nacional de Desenvolvimento da Educa&ccedil;&atilde;o &ndash; FNDE a realizar a transfer&ecirc;ncia de recursos financeiros aos munic&iacute;pios e ao Distrito Federal para a manuten&ccedil;&atilde;o de novas matr&iacute;culas em novos estabelecimentos p&uacute;blicos de educa&ccedil;&atilde;o infantil, constru&iacute;dos com recursos de programas federais, conforme Resolu&ccedil;&atilde;o CD/FNDE n&ordm; 15 de 16 de maio de 2013.</p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: justify;"><strong>O SECRET&Aacute;RIO DE EDUCA&Ccedil;&Atilde;O B&Aacute;SICA</strong>, no uso das atribui&ccedil;&otilde;es, resolve:</p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: justify;">Art. 1&ordm; Divulgar os munic&iacute;pios e o Distrito Federal que est&atilde;o aptos a receber o pagamento do recurso de apoio &agrave; manuten&ccedil;&atilde;o de novas matr&iacute;culas em novos estabelecimentos p&uacute;blicos de educa&ccedil;&atilde;o infantil, constru&iacute;dos com recursos de programas federais, que estejam em plena atividade e com matr&iacute;culas que ainda n&atilde;o tenham sido contempladas com recursos do Fundo de Manuten&ccedil;&atilde;o e Desenvolvimento da Educa&ccedil;&atilde;o B&aacute;sica e de Valoriza&ccedil;&atilde;o dos Profissionais da Educa&ccedil;&atilde;o (Fundeb), de que trata a Lei n&ordm; 12.499 de 29 de setembro de 2011, e conforme informa&ccedil;&otilde;es declaradas pelos munic&iacute;pios e o Distrito Federal no SIMEC &ndash; M&oacute;dulo E.I. Manuten&ccedil;&atilde;o &ndash; Unidades do Proinf&acirc;ncia.</p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: justify;">Art. 2&ordm; Autorizar o FNDE/MEC a realizar a transfer&ecirc;ncia de recursos financeiros aos munic&iacute;pios e Distrito Federal para manuten&ccedil;&atilde;o de novas matr&iacute;culas em novos estabelecimentos p&uacute;blicos de educa&ccedil;&atilde;o infantil, conforme destinat&aacute;rios e valores constantes da listagem anexa.</p>
<p style="text-align: justify;">&nbsp;</p>
<p style="text-align: justify;">Art. 3&ordm; Esta Portaria entra em vigor na data de sua publica&ccedil;&atilde;o</p>
<p>&nbsp;</p>
<p>&nbsp;</p>
<p style="text-align: center;"><strong>MANUEL FERNANDO PALÁCIOS DA CUNHA E MELO</strong></p>
<p style="text-align: center;">Secret&aacute;rio da Educa&ccedil;&atilde;o B&aacute;sica</p>';
		
		$textoSQL = simec_htmlspecialchars($texto, ENT_QUOTES);
		
		$sql = "INSERT INTO proinfantil.loteproinfancia(usucpf, lotnumportaria, lotdataportaria, lotminutaportaria) 
				VALUES ('".$_SESSION['usucpf']."', {$lotnumportaria}, '".formata_data_sql($lotdataportaria)."', '{$textoSQL}') RETURNING lotid;";
		$lotid = $db->pegaUm($sql);
				
		foreach( $pinid as $id ){
			$sql = "UPDATE proinfantil.proinfantil SET lotid = $lotid WHERE pinid = $id";
			$db->executar($sql);
			wf_alterarEstado( $docid[$id], AEDID_PRO_ENCAMINHAR_AGUARDANDO_PAGAMENTO, 'Tramitação em Lote', array( ) );
		}
		
		$html = $texto.'
		<p style="page-break-before:always"><!-- pagebreak --></p>
		<table align="center" class="listagem" border="1" width="100%" cellSpacing="1" cellPadding=3 >
			<tr>
				<th colspan="8" style="text-align: center;">ANEXO</th>
			</tr>
			<tr>
				<th rowspan="2" width="05%"><b>UF</b></th>
				<th rowspan="2" width="25%" style="text-align: center;"><b>Municípios</b></th>
				<th rowspan="2" width="05%" style="text-align: center;"><b>Código IBGE</b></th>
				<th colspan="4" width="60%" style="text-align: justify;"><b>Quantidade de novas matrículas, declaradas pelos Municípios e o Distrito Federal, em novos estabelecimentos públicos de educação infantil, construídos com recursos de programas
	federais e que estão em plena atividade</b></th>
				<th rowspan="2" width="05%" style="text-align: center;"><b>Valor do Repasse</b></th>
			</tr>
			<tr>
				<th style="text-align: center;"><b>Creche Parcial</b></th>
				<th style="text-align: center;"><b>Creche Integral</b></th>
				<th style="text-align: center;"><b>Pré-Escola Parcial</b></th>
				<th style="text-align: center;"><b>Pré-Escola Integral</b></th>
			</tr>';
			
			$sql = "SELECT distinct 
						tm.estuf, 
					    tm.muncod, 
					    tm.mundescricao,
					    p.pinid,
					    case when p.pinanoseguinte = 'S' then p.pinperiodorepasse2 else p.pinperiodorepasse end as pinperiodorepasse,
					    p.lotid,
					    lp.lotdsc
					FROM
						proinfantil.proinfantil p
					    inner join proinfantil.loteproinfancia lp on lp.lotid = p.lotid
					    inner join obras2.obras oi on oi.obrid = p.obrid and oi.obrstatus = 'A'
					    inner join entidade.entidade AS ee ON oi.entid = ee.entid
					    inner join entidade.endereco AS ed ON oi.endid = ed.endid
					    inner join territorios.municipio AS tm ON ed.muncod = tm.muncod
					WHERE p.lotid = $lotid
					order by tm.estuf, tm.mundescricao";
			
			$arrDados = $db->carregar($sql);
			$arrDados = $arrDados ? $arrDados : array();
			//ver($arrDados,d);
			$totMatricula = 0;
			$lote = $lotid;
			$arrLote = array();
			$arrMuncod = array();
			$muncodAnterior	= '';
			$valorTotalGeral = 0;
			
			foreach ($arrDados as $key => $v) {
				$muncodAtual 	= $v['muncod'];
				$muncodProximo 	= $arrDados[$key+1]['muncod'];
								
				$dataini = recuperaDataInicioAtendimento($v['pinid']);
				$data_atendimento = formata_data_sql($dataini);
				$vlPeriodoMes = 0;
				$totMatricula = (int)0;
				$valorTotal = (int)0;
				
				$qtdChecParc = $db->pegaUm("SELECT coalesce(sum(mds.alaquantidade), 0) FROM proinfantil.mdsalunoatendidopbf mds where mds.titid in (2) and mds.pinid = {$v['pinid']} and mds.timid  = 3");
				if( $qtdChecParc > 0 ){
					$totMatricula =(int)$qtdChecParc;
					$sql = "SELECT vaavalor as valor FROM proinfantil.valoraluno 
						WHERE vaastatus = 'A' AND vaatipo = 'P' 
						AND timid = 3 AND ('{$data_atendimento}' BETWEEN vaadatainicial AND vaadatafinal)";
					$vlPeriodo = $db->pegaUm($sql);
					$vlPeriodoMes = $vlPeriodo ? $vlPeriodo / 12 : 0;				
					$valorTotal = ($totMatricula * ($vlPeriodoMes ? $vlPeriodoMes : 0) * $v['pinperiodorepasse']);
				}
				$valorTotalGeral+=(float)$valorTotal;
				$valorTotal = (int)0;
				
				$qtdChecInte = $db->pegaUm("SELECT coalesce(sum(mds.alaquantidade), 0) FROM proinfantil.mdsalunoatendidopbf mds where mds.titid in (1) and mds.pinid = {$v['pinid']} and mds.timid  = 3");
				if( $qtdChecInte > 0 ){
					$totMatricula =(int)$qtdChecInte;
					$sql = "SELECT vaavalor as valor FROM proinfantil.valoraluno 
						WHERE vaastatus = 'A' AND vaatipo = 'I' 
						AND timid = 3 AND ('{$data_atendimento}' BETWEEN vaadatainicial AND vaadatafinal)";
					$vlPeriodo = $db->pegaUm($sql);
					$vlPeriodoMes = $vlPeriodo ? $vlPeriodo / 12 : 0;			
					$valorTotal = ($totMatricula * ($vlPeriodoMes ? $vlPeriodoMes : 0) * $v['pinperiodorepasse']);
				}
				$valorTotalGeral+=(float)$valorTotal;
				$valorTotal = (int)0;
				
				$qtdPreParc = $db->pegaUm("SELECT coalesce(sum(mds.alaquantidade), 0) FROM proinfantil.mdsalunoatendidopbf mds where mds.titid in (2) and mds.pinid = {$v['pinid']} and mds.timid  = 1");
				if( $qtdPreParc > 0 ){
					$totMatricula =(int)$qtdPreParc;
					$sql = "SELECT vaavalor as valor FROM proinfantil.valoraluno 
						WHERE vaastatus = 'A' AND vaatipo = 'P' 
						AND timid = 1 AND ('{$data_atendimento}' BETWEEN vaadatainicial AND vaadatafinal)";
					$vlPeriodo = $db->pegaUm($sql);
					$vlPeriodoMes = $vlPeriodo ? $vlPeriodo / 12 : 0;				
					$valorTotal+= ($totMatricula * ($vlPeriodoMes ? $vlPeriodoMes : 0) * $v['pinperiodorepasse']);
				}
				$valorTotalGeral+=(float)$valorTotal;
				$valorTotal = (int)0;
				
				$qtdPreInte = $db->pegaUm("SELECT coalesce(sum(mds.alaquantidade), 0) FROM proinfantil.mdsalunoatendidopbf mds where mds.titid in (1) and mds.pinid = {$v['pinid']} and mds.timid  = 1");
				if( $qtdPreInte > 0 ){
					$totMatricula =(int)$qtdPreInte;
					$sql = "SELECT vaavalor as valor FROM proinfantil.valoraluno 
						WHERE vaastatus = 'A' AND vaatipo = 'I' 
						AND timid = 1 AND ('{$data_atendimento}' BETWEEN vaadatainicial AND vaadatafinal)";
					$vlPeriodo = $db->pegaUm($sql);
					$vlPeriodoMes = $vlPeriodo ? $vlPeriodo / 12 : 0;				
					$valorTotal+= ($totMatricula * ($vlPeriodoMes ? $vlPeriodoMes : 0) * $v['pinperiodorepasse']);
				}
				$valorTotalGeral+=(float)$valorTotal;
				$valorTotal = (int)0;
				
				$qtdChecParcTot += $qtdChecParc;
				$qtdChecInteTot += $qtdChecInte;
				$qtdPreParcTot 	+= $qtdPreParc;
				$qtdPreInteTot 	+= $qtdPreInte;
				
				if( $muncodProximo != $muncodAtual ){
					
					$arrLote[]= array(
									'municipio' 	=> $v['mundescricao'],
									'ibge' 			=> $v['muncod'],
									'estuf' 		=> $v['estuf'],
									'qtdChecParc'	=> (int)$qtdChecParcTot,
									'qtdChecInte' 	=> (int)$qtdChecInteTot,
									'qtdPreParc' 	=> (int)$qtdPreParcTot,
									'qtdPreInte' 	=> (int)$qtdPreInteTot,
									'valorTotal' 	=> (float)$valorTotalGeral,
								);
					$qtdChecParcTot 	= 0;
					$qtdChecInteTot 	= 0;
					$qtdPreParcTot 		= 0;
					$qtdPreInteTot		= 0;
					$valorTotalGeral 	= 0;
				}
				$totMatricula = (int)0;
			}
		//ver($arrLote,d);
		
		foreach ($arrLote as $v) {
			extract($v);
			$valorTotal = ($valorTotal ? number_format($valorTotal,2,",",".") : '0,00');
				
			$html.='
			<tr>
				<td>'.$v['estuf'].'</td>
				<td style="text-align: left;">'.$municipio.'</td>
				<td style="text-align: center;">'.$ibge.'</td>
				<td style="text-align: center;">'.(int)$qtdChecParc.'</td>
				<td style="text-align: center;">'.(int)$qtdChecInte.'</td>
				<td style="text-align: center;">'.(int)$qtdPreParc.'</td>
				<td style="text-align: center;">'.(int)$qtdPreInte.'</td>
				<td style="text-align: right;">'.$valorTotal.'</td>
			</tr>';
			
			$valorTotal = str_replace(".","", $valorTotal);
			$valorTotal = str_replace(",",".", $valorTotal);
			
			$sql = "INSERT INTO proinfantil.loteminutaproinfantil(lotid, estuf, muncod, crecheparcial, crecheintegral, preescolaparcial, preescolaintegral, valorrepasse) 
					VALUES ({$lotid}, '{$estuf}', '{$ibge}', ".(int)$qtdChecParc.", ".(int)$qtdChecInte.", ".(int)$qtdPreParc.", ".(int)$qtdPreInte.", ".$valorTotal.")";
			$db->executar($sql);
		}
		
		$html.= '</table>';
		
		include_once APPRAIZ . "includes/classes/RequestHttp.class.inc";
		ob_clean();
			
		$nomeArquivo 		= 'minuta_repasse_'.date('Y-m-d').'_lote_'.$lote;
		$diretorio		 	= APPRAIZ . 'arquivos/proinfantil/minutaproinfantil';
		$diretorioArquivo 	= APPRAIZ . 'arquivos/proinfantil/minutaproinfantil/'.$nomeArquivo.'.pdf';
		
		if( !is_dir($diretorio) ){
			mkdir($diretorio, 0777);
		}
		
		$http = new RequestHttp();
		$html = utf8_encode($html);
		$response = $http->toPdf( $html );
	
		$fp = fopen($diretorioArquivo, "w");
		if ($fp) {
		  stream_set_write_buffer($fp, 0);
		  fwrite($fp, $response);
		  fclose($fp);
		}
		
		$sql = "INSERT INTO public.arquivo (arqnome, arqextensao, arqdescricao, arqtipo, arqtamanho, arqdata, arqhora, usucpf, sisid, arqstatus)
				VALUES( '".$nomeArquivo."',
						'pdf',
						'".$nomeArquivo."',
						'application/pdf',
						'".filesize($diretorioArquivo)."',
						'".date('Y-m-d')."',
						'".date('H:i:s')."',
						'".$_SESSION["usucpf"]."',
						{$_SESSION['sisid']},
						'A') RETURNING arqid";
		
		$arqid = $db->pegaUm($sql);

		$sql = "UPDATE proinfantil.loteproinfancia SET arqid = $arqid, lotdsc = 'Lote: '||lotid WHERE lotid = $lotid";
		$db->executar($sql);
		$db->commit();
	}
	echo "<script>
			alert('Lote criado com sucesso.');
			window.location = 'proinfantil.php?modulo=principal/pagamentoLote&acao=A';
		  </script>";
}

function confirmaPagamentoLote( $request ){
	
	global $db;
	
	require_once APPRAIZ . 'includes/workflow.php';
	
	extract($request);
	
	$sql = "SELECT DISTINCT
				dcm.docid
			FROM 
				proinfantil.proinfantil dcm 
			INNER JOIN workflow.documento doc ON doc.docid = dcm.docid AND doc.esdid = ".WF_PROINFANTIL_ENVIADO_PARA_PAGAMENTO."	
			WHERE
				lotid = $lotid";
	$docids = $db->carregarColuna($sql);
	
	if( $docids[0] != '' ){
		foreach( $docids as $docid ){
			wf_alterarEstado( $docid, AEDID_PRO_ENCAMINHAR_PAGAMENTO_EFETUADO, 'Tramitação em Lote - Confirmar Pagamento', array( ) );
		}
	}
	echo "<script>
			alert('Pagamento confirmado.');
			window.location = 'proinfantil.php?modulo=principal/pagamentoLote&acao=A';
		  </script>";
}

function confirmaPagamentoMunicipio( $request ){
	
	global $db;
	
	require_once APPRAIZ . 'includes/workflow.php';
	
	extract($request);
	
	if( $docid[0] != '' ){
		foreach( $docid as $id ){
			wf_alterarEstado( $id, AEDID_PRO_ENCAMINHAR_PAGAMENTO_EFETUADO, 'Tramitação em Lote - Confirmar Pagamento', array( ) );
		}
	}
	echo "<script>
			alert('Pagamento confirmado.');
			window.location = 'proinfantil.php?modulo=principal/pagamentoLote&acao=A';
		  </script>";
}

function arquivarProInfancia(){
	
	global $db;
	
	require_once APPRAIZ . 'includes/workflow.php';
	
	$docid = recuperaDocumentoProInfantil($_SESSION['proinfantil']['pinid']);
// 	ver($_SESSION,d);
	
	if( wf_alterarEstado( $docid, AEDID_PRO_CADASTRAMENTO_INDEFERIR, $_REQUEST['cmddsc'], array() ) ){

		$sql = "UPDATE proinfantil.proinfanciaanalise SET prastatus = 'I' WHERE pinid = {$_SESSION['proinfantil']['pinid']} AND praarquivada IS TRUE;
				INSERT INTO proinfantil.proinfanciaanalise(prapareceraprovacao, prastatus, pradata, usucpf, pinid, praarquivada)
				VALUES( '{$_REQUEST['cmddsc']}', 'A', now(), '{$_SESSION['usucpf']}', {$_SESSION['proinfantil']['pinid']}, true);";
		$db->executar($sql);
		$db->commit();
		
		echo "<script>
				alert('Obra Arquivada.');
				window.location = 'proinfantil.php?modulo=principal/listaObrasMunicipio&acao=A';
			  </script>";
		
	}else{
		echo "<script>
					alert('Erro no Arquivamento.');
					window.location = window.location;
				  </script>";
	}
}

function verificarAdesaoProInfancia($pinid){
	global $db;
	$sql = "SELECT 		trim(pinadesaopresposta)
			FROM 		proinfantil.proinfantil
			WHERE		pinid = {$pinid}";
	
	$adpresposta = $db->pegaUm($sql);
	
	if($adpresposta == 'S'){
		return 'S';
	} else {
		$sql = "SELECT 		praarquivada
				FROM 		proinfantil.proinfanciaanalise
				WHERE		pinid = {$pinid} AND praarquivada IS TRUE AND prastatus = 'A'";
		
		$arquivada = $db->pegaUm($sql);
		if( $arquivada == 't' ){
			return 'A';
		}else{
			return 'N';
		}
	} 
}

function montaAbasProinfantil($abacod_tela, $url, $parametros = '', $quest = ''){
	global $db;
		
	$perfis = pegaPerfil($_SESSION["usucpf"]);
	$arMnuid = array();
	
	/*$docid = recuperaDocumentoProInfantil($_SESSION['proinfantil']['pinid']);
	$esdid = pegaEstadoAtualDocumento($docid);
	$atual = pegaEstadoAtualDocumento($docid);*/
	
	#Verifica que habilita a aba analise 2, retorna 'S' ou 'N'
	$abaAnalise = verificarAbaAnalise($_SESSION['proinfantil']['pinid']);
		
	if($quest=='S'){
		$arMnuid[] =  7791; //Crianças Atendidas
		$arMnuid[] =  7792; //Fotos
		$arMnuid[] = 13166; //Análise	
		$arMnuid[] = 13160; //Repasse
	}

	#Escoder as abas de analise ano 1 e ano 2
	if( $abaAnalise == 'N' ){
		$arMnuid[] = 10852; //Análise Ano 1
		$arMnuid[] = 10992; //Análise Ano 2
	} else {
		$arMnuid[] = 13166; //Análise
	}
	
	if( (in_array(CONSULTA_GERAL,$perfis) || in_array(PERFIL_COORDENADOR,$perfis) || in_array(EQUIPE_MUNICIPAL,$perfis) || in_array(SECRETARIO_ESTADUAL,$perfis)) && empty($abaAnalise)){
		$arMnuid[] = 13160; //Repasse
	}
	if( !empty($_SESSION['proinfantil']['popup']) ) $arMnuid[] = 7789;

	return $db->cria_aba($abacod_tela, $url, $parametros, $arMnuid);
} 
?>