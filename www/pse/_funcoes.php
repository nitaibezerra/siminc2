<?php 
$espid = $_SESSION['pse']['espid'];
//ver ($espid, $_SESSION['pse']['espid'], d);
function pegaPerfil( $usucpf ){
	global $db;
	$sql = "SELECT pu.pflcod
			FROM seguranca.perfil AS p LEFT JOIN seguranca.perfilusuario AS pu 
			  ON pu.pflcod = p.pflcod
			WHERE 
			  p.sisid = '{$_SESSION['sisid']}'
			  AND pu.usucpf = '$usucpf'";	
	
	$pflcod = $db->pegaUm( $sql );
	return $pflcod;
}

function VerSessao(){
	if(empty($_SESSION['pse']['entid']) || empty($_SESSION['pse']['espid']) ){
		echo "alert('teste na sessão!')";
		echo "<script>window.location.href='pse.php?modulo=inicio&acao=C';</script>";
		exit;
	}
}

function filtraMunicipio($estuf){
	global $db;
	$sql = "SELECT
				ter.muncod AS codigo,
				ter.mundescricao AS descricao
			FROM
				territorios.municipio ter
			WHERE
				ter.estuf = '$estuf'
			ORDER BY ter.mundescricao";
		
	echo $db->monta_combo( "muncod", $sql, 'S', 'Selecione...', '', '', '', '215', 'N','id="muncod"');

}

/*
function BuscaDadosINEP($entcodent){
	global $db;
				
		$sql = "select escola.entid, escola.entcodent, escola.entnome, professor.entnome as dir,ed.endlog||', '||ed.endcom||' - '||ed.endbai as endereco 
				from entidade.entidade escola
				inner join entidade.endereco ed on ed.entid = escola.entid
				inner join entidade.funcaoentidade feEscola on feEscola.entid = escola.entid
				left join entidade.funentassoc assocprofessores on assocprofessores.entid = escola.entid
				inner join entidade.funcaoentidade feProfessor on feProfessor.fueid = assocprofessores.fueid	
				inner join entidade.entidade professor on professor.entid = feProfessor.entid
				where feEscola.funid in (3,4)	
				and ( 
					feProfessor.funid = 19 or
					feProfessor.funid = 22
				)
				and escola.entcodent = '$entcodent'";
				
		$sql ="	SELECT ent.entid,ent.entcodent, ent.entnome, ende.estuf, mun.mundescricao,coalesce(ende.endlog, '')||', '||coalesce(ende.endcom, '')||' - '||coalesce(ende.endbai, '') as endereco, professor.entid as identf, professor.entnome as dir  
				FROM entidade.entidade ent				
				LEFT JOIN 
					entidade.funcaoentidade feEscola on feEscola.entid = ent.entid
				LEFT JOIN 
					entidade.funentassoc assocprofessores on assocprofessores.entid = ent.entid
				LEFT JOIN 
					entidade.funcaoentidade feProfessor on feProfessor.fueid = assocprofessores.fueid	
				LEFT JOIN 
					entidade.entidade professor on professor.entid = feProfessor.entid
				LEFT JOIN
					entidade.endereco ende ON ende.entid = ent.entid
				INNER JOIN
					entidade.entidade2 ent2 ON ent2.entid = ent.entid
				LEFT JOIN
					entidade.entidadedetalhe entd ON entd.entid = ent.entid		
				INNER JOIN
					territorios.municipio mun ON mun.muncod = ende.muncod
				INNER JOIN
					territorios.estado est ON est.estuf = mun.estuf
				WHERE	ent.entstatus='A' AND 
						ent2.funid = 3
						and ent.entcodent = '$entcodent'";
				
		$dados = $db->pegalinha($sql);
		$entid		= (($dados['entid']==NULL)?'':$dados['entid']);
		$nome		= (($dados['entnome']==NULL)?'':$dados['entnome']);
		$dir		= (($dados['dir']==NULL)?'':$dados['dir']);
		$endereco	= (($dados['endereco']==NULL)?'':$dados['endereco']);
		 
		if(!empty($dados))
			echo $entid."|".$nome."|".$dir."|".$endereco;
		else
			echo 0;

}
*/

function verEquipesPSE($cnes,$entid,$equipe){
	$equipe = ($equipe=='undefined'?'':$equipe);
	$cnes = ($cnes=='undefined'?'':$cnes);
	global $db;
	if($cnes){
		$sql = "SELECT	cneid as codigo,
						'Equipe nr: '||cneseqequipe as descricao		
				FROM 	pse.scnes
				WHERE	cnecodigocnes = $cnes
				ORDER BY cneseqequipe";
		$dados = $db->carregar($sql);
		$quant = count($dados);
		$hab = 'S';
		
		$db->monta_combo( "nrequip", $sql, $hab, 'Selecione uma Equipe', 'teste', '', '', '215', 'N','nrequip','',$equipe);
	}
	else if ($cnes == '') {
		print "<script>"
			. "    alert('Faltam dados para esta tela! Tente executar a operação novamente.');"
			. "    history.back(-1);"
			. "</script>";
		
		die;
	}
}

function verEquipesAjax($cnes,$entid,$equipe){
	$equipe = ($equipe=='undefined'?'':$equipe);
	global $db;
	$sql = "SELECT	cneid as codigo,
					'Equipe nr: '||cneseqequipe as descricao		
			FROM 	pse.scnes
			WHERE	cnecodigocnes = $cnes
			ORDER BY cneseqequipe";
	$dados = $db->carregar($sql);
	$quant = count($dados);
	if($quant>1 && $equipe=='')
		$hab = 'S';
	else
		$hab = 'N';	
	
	$db->monta_combo( "nrequip", $sql, $hab, '', '', '', '', '215', 'N','nrequip','',$equipe);
}

function listaEquipeSaudeFamilia($entid, $tt = null){
	global $db;
	$usucpf = $_SESSION['usucpf'];
	$pflcod = pegaPerfil($usucpf);
	
	$sql = "SELECT * FROM pse.programacaoexercicio WHERE prsano = '".$_SESSION['exercicio']."'";
	$arr = $db->pegaLinha( $sql );
	$dataAtual = date("Y-m-d");
	$data = new Data();
	$dataF = trim($arr['prsdata_termino']);
	$resp = 1;
		if( !empty($dataF) ){
			$resp = $data->diferencaEntreDatas($dataAtual, $arr['prsdata_termino'], 'maiorDataBolean','','');		
		}
	$sql = "SELECT	";
	if( $resp == NULL ){
		if($pflcod == ESCOLA_ESTADUAL || $pflcod == ESCOLA_MUNICIPAL || $pflcod == SUPER_USUARIO ){
			if($tt == ''){
				$sql.=	"'<a href=\"#\" onclick=\"AlterarEquipe(\'' || esf.esfid || '\');\" title=\"Alterar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>
					<a href=\"#\" onclick=\"ExcluirEquipe(\'' || esf.esfid || '\');\" title=\"Excluir\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'";
			} else {
				$sql.= "'-'";
			}
		} else {
			$sql.= "'-'";
		}
	} else {
		$sql.= "'-'";
	}
	$sql.= "as acao, sc.cnecodigocnes, sc.cneseqequipe, sc.cnenomefantasia,esf.esfendereco
			FROM pse.equipesaudefamilia esf, pse.scnes sc, pse.escolapse ep
			WHERE	esf.cneid = sc.cneid and
					esf.espid = ep.espid and
					ep.entid = $entid and
	 				ep.espano = ".$_SESSION['exercicio'];
	$cabecalho = array( "Ação", "SCNES", "Nº Equipe", "Nome", "Endereço da Equipe");
	$alinha = array("center","center","center","left","left");
	$tamanho = array("5%","10%","10%","30%","45%");
	$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', '', '',$tamanho,$alinha);
}

function salvarEquipeSaudeFamilia($post){
	global $db;
		if(!empty($post['idequipe'])){
			$endereco = utf8_decode($_REQUEST['endequipe']);
			$sql = "UPDATE pse.equipesaudefamilia set
					cneid=$post[idcnes],
					esfendereco='$endereco'
					WHERE 
					esfid=$post[idequipe]";
			$db->executar($sql);
			$db->commit();
			echo 1;
		}
		else {
			$sql = "select count(*)
					from pse.equipesaudefamilia esf, pse.escolapse esp
					where esp.espid=esf.espid and cneid=$post[idcnes] and esp.entid = $post[entid] and espano = ".$_SESSION['exercicio'];
			$quant = $db->pegaum($sql);
			if($quant==0){		
				$sql = "INSERT INTO pse.equipesaudefamilia
						(cneid,esfendereco,espid) 
						VALUES 
						($post[idcnes],'$endereco',$post[espid])";
				$db->executar($sql);
				$db->commit();
				echo 1;
			}
			else
				echo "A Equipe informada já está cadastrada!";
		}

}

function excluiEquipeSaudeFamilia($id){
	global $db;
		$sql = "select count(*) from pse.pseanoescola
				where esfid=$id";
		$quant = $db->pegaum($sql);
		if($quant==0){	
			$sql = "delete from pse.equipesaudefamilia
					where esfid=$id";
			$db->executar($sql);
			$db->commit();
			echo 1;
		}
		else
			echo "A Equipe não pode ser excluída!";
}
	
function alteraPSE2009($id){
	global $db;
		$sql = "select pamanoreferencia, empid, esfid, moeid, nieid, pamquantprevista, pamquantatendida, pam.entid, scnes.cnecodigocnes, ent.entcodent from pse.pseanoescolamun pam
				INNER JOIN pse.scnes AS scnes ON scnes.cneid = pam.esfid
				INNER JOIN entidade.entidade AS ent ON ent.entid = pam.entid
				where pamid = $id";
		$dados = $db->pegalinha($sql);
		
		echo $dados['pamanoreferencia']."|".$dados['empid']."|".$dados['esfid']."|".$dados['moeid']."|".$dados['nieid']."|".$dados['pamquantprevista']."|".$dados['pamquantatendida']."|".$dados['entid']."|".$id."|".$dados['cnecodigocnes']."|".$dados['entcodent'];
}

function alteraEquipeSaudeFamilia($id){
	global $db;
		$sql = "select e.esfid, e.cneid, e.esfendereco, c.cnecodigocnes,c.cneseqequipe
				from pse.equipesaudefamilia e, pse.scnes c
				where e.cneid = c.cneid and e.esfid=$id";
		$dados = $db->pegalinha($sql);
		
		echo $dados['cneid']."|".$dados['cnecodigocnes']."|".$dados['esfendereco']."|".$id."|".$dados['cneseqequipe'];
}



function listaEducadorReferencia($entid, $tt = null){
	global $db;
	$usucpf = $_SESSION['usucpf'];
	$pflcod = pegaPerfil($usucpf);
	
	$sql = "SELECT * FROM pse.programacaoexercicio WHERE prsano = '".$_SESSION['exercicio']."'";
	$arr = $db->pegaLinha( $sql );
	$dataAtual = date("Y-m-d");
	$data = new Data();
	$dataF = trim($arr['prsdata_termino']);
	$resp = 1;
		if( !empty($dataF) ){
			$resp = $data->diferencaEntreDatas($dataAtual, $arr['prsdata_termino'], 'maiorDataBolean','','');		
		}
	$sql = "SELECT	";
	if( $resp == NULL ){
		if($pflcod == ESCOLA_ESTADUAL || $pflcod == ESCOLA_MUNICIPAL || $pflcod == SUPER_USUARIO){
			if($tt == ''){
				$sql.=	"'<a href=\"#\" onclick=\"AlterarEducador(\'' || e.edrid || '\');\" title=\"Alterar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>
					<a href=\"#\" onclick=\"ExcluirEducador(\'' || e.edrid || '\');\" title=\"Excluir\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'";
			} else {
				$sql.= "'-'";
			}
		}
		else {
			$sql.= "'-'";
		}
	} else {
		$sql.= "'-'";
	}
	$sql.= "as acao, e.edrcpf,e.edrnome,e.edrendereco
			FROM pse.educadoreferencia e, pse.escolapse es
			WHERE e.espid=es.espid and es.entid = $entid and es.espano = ".$_SESSION['exercicio'];
		$quant = $db->pegalinha($sql);			
		//if(!empty($quant)){
			$cabecalho 		= array( "Ação", "CPF", "Nome", "Endereço");
			$alinha = array("center","center","left","left");
			$tamanho = array("5%","10%","35%","50%");
			$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', '', '',$tamanho,$alinha);

		//}
}

function salvarEducadorReferencia($post){
	$cpf = $_REQUEST['cpfEducador'];
	$cpf = str_replace('.','',$cpf);
	$cpf = str_replace('-','',$cpf);
	$cep = str_replace('-','',$_REQUEST['cepEducador']);

	$outrocargo = utf8_decode($_REQUEST['outrocargoeducador']);
	$endereco = utf8_decode($_REQUEST['enderecoEducador']);
	$nome = utf8_decode($_REQUEST['nomeEducador']);
	$mail = utf8_decode($_REQUEST['emailEducador']);
	$rg = utf8_decode($_REQUEST['rgEducador']);
		
	$espid = $_SESSION['pse']['espid'];
		
	global $db;
		if(!empty($_REQUEST['ideducador'])){
			$sql = "UPDATE pse.educadoreferencia set
					cafid=$_REQUEST[funcEducador],
					edrnome='$nome',
					edrendereco='$endereco',
					edrcep=$cep,
					edrtelefone='$_REQUEST[telEducador]',
					edremail='$mail',
					edrrg='$rg',
					edrcpf='$cpf',
					edroutrocargo='$outrocargo'
					WHERE 
					edrid = $_REQUEST[ideducador]";					
			$db->executar($sql);
			$db->commit();
			echo 1;
		}
		else {
			$sql = "select count(ed.edrid) from pse.educadoreferencia ed
					inner join pse.escolapse es on es.espid = ed.espid
					where
						( ed.edrnome='$nome' OR
						  -- edremail='$mail' OR
						  ed.edrrg='$_REQUEST[rgEducador]' OR
						  ed.edrcpf='$cpf' ) AND 
						  ed.espid = $espid AND
						  es.espano = ".$_SESSION['exercicio'];
			
			$quant = $db->pegaum($sql);
			if($quant==0){	
				$sql = "INSERT INTO pse.educadoreferencia
						(cafid,espid,edrnome,
						 edrendereco,edrcep,edrtelefone,
						 edremail,edrrg,edrcpf,edroutrocargo) 
						VALUES 
						($_REQUEST[funcEducador],$espid,'$nome',
						'$endereco',$cep,'$_REQUEST[telEducador]',
						'$mail','$_REQUEST[rgEducador]','$cpf','$outrocargo')";
				$db->executar($sql);
				$db->commit();
				echo 1;
			}
			else
				echo "Dados já informado!";
		}
		
}

function excluiEducador($id){
	global $db;
			$sql = "delete from pse.educadoreferencia
					where edrid=$id";
			$db->executar($sql);
			$db->commit();
			echo 1;
}


function alteraEducador($id){
	global $db;
		$sql = "select edrid,cafid,edrnome,edrendereco,edrcep,
  					   edrtelefone,edremail,edroutrocargo,edrrg,edrcpf
				from pse.educadoreferencia
				where edrid=$id";
		$dados = $db->pegalinha($sql);
				
		$outrocargo = $dados['edroutrocargo'];
		$endereco = $dados['edrendereco'];
		$nome = $dados['edrnome'];
		
		echo $dados['edrid']."|".$dados['cafid']."|".$nome."|".$endereco."|".$dados['edrcep']."|".$dados['edrtelefone']."|".$dados['edremail']."|".$outrocargo."|".$dados['edrrg']."|".$dados['edrcpf'];
		
}


/*function identificaESF($entid){
	global $db;
	$sql = "SELECT	esf.esfid as codigo, sc.cnecodigocnes||' - '||'Equipe '||sc.cneseqequipe||' - '||sc.cnenomefantasia as descricao
			FROM	pse.equipesaudefamilia esf, pse.scnes sc, pse.escolapse es
			WHERE	esf.cneid = sc.cneid and esf.espid=es.espid";
	$db->monta_combo( "identESF", $sql, 'S', 'Selecione', 'iesf', '', '', '215', 'S','identESF');
}*/

function listaQuantitativo($entid,$ano, $tt = null){
	
	global $db;
	$ano = ($ano==''?0:$ano);
	$campo = ($ano==$_SESSION['exercicio']?',pae.paequantatendida':'');
	
	$usucpf = $_SESSION['usucpf'];
	$pflcod = pegaPerfil($usucpf);
	
	$sql = "SELECT * FROM pse.programacaoexercicio WHERE prsano = '".$_SESSION['exercicio']."'";
	$arr = $db->pegaLinha( $sql );
	$dataAtual = date("Y-m-d");
	$data = new Data();
	$dataF = trim($arr['prsdata_termino']);
	$resp = 1;
		if( !empty($dataF) ){
			$resp = $data->diferencaEntreDatas($dataAtual, $arr['prsdata_termino'], 'maiorDataBolean','','');		
		}
	$sql = "SELECT	";
	if( $resp == NULL ){
		if($pflcod == ESCOLA_ESTADUAL || $pflcod == ESCOLA_MUNICIPAL || $pflcod == SUPER_USUARIO){
			if($tt == ''){
				$sql.=	"'<a href=\"#\" onclick=\"Alterarquant(\'' || pae.paeid || '\');\" title=\"Alterar\"><img src=\"../imagens/alterar.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'";
				if ( $_SESSION['exercicio'] == $ano ){
							$sql.= "'<a href=\"#\" onclick=\"Excluirquant(\'' || pae.paeid || '\');\" title=\"Excluir\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'";
				}
			} else {
				$sql.= "'-'";
			}
		} else {
			$sql.= "'-'";
		}
	} else {
		$sql.= "'-'";
	}
	$sql.= "as acao, cne.cnecodigocnes||' - '||cne.cnenomefantasia,
					cne.cneseqequipe,
					mod.moedsc,
					niv.niedsc,
					pae.paequantprevista
					$campo
			FROM pse.pseanoescola pae
			INNER JOIN
				pse.modalidadeensino mod on mod.moeid=pae.moeid
			INNER JOIN
				pse.nivelensino niv on niv.nieid=pae.nieid
			INNER JOIN
				pse.equipesaudefamilia esf on esf.esfid=pae.esfid
			INNER JOIN
				pse.escolapse esp on esp.espid=esf.espid
			INNER JOIN
				pse.scnes cne on cne.cneid=esf.cneid
			WHERE esp.entid = $entid AND pae.paeanoreferencia = $ano";
			if($ano>0)	
				echo "<b>ANO: ".$ano."</b>";
			if($ano==$_SESSION['exercicio']){		
				$cabecalho 	= array( "Ação", "CNES/Nome","Nº Equipe", "Modalidade","Nível de Ensino", "Qd. Prevista", "Qd. Atendida");
				$alinha = array("center","left","center","left","left","center","center");
				$tamanho = array("5%","25%","10%","20%","20%","10%","10%");
			}
			else {		
				$cabecalho 	= array( "Ação", "CNES/Nome","Nº Equipe", "Modalidade","Nível de Ensino", "Qd. Prevista");
				$alinha = array("center","left","center","left","left","center");
				$tamanho = array("5%","25%","10%","20%","20%","10%");
			}	
			$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', '', '',$tamanho,$alinha);
}

function salvarQuantitativo($post){
	global $db;
		
		$atendidos = ($_REQUEST['ano']==$_SESSION['exercicio']?$_REQUEST['quantEstAten']:'NULL');
		$qtdprevista = $_REQUEST['quantEstPrev']?$_REQUEST['quantEstPrev']:0;
		if(!empty($_REQUEST['paeid'])){
			
			if( $_REQUEST['espid'] == $_REQUEST['novoespid'] ){
				$where = " paeanoreferencia=$_REQUEST[ano], 
							esfid=$_REQUEST[idesf],  
							moeid=$_REQUEST[modalidadeEnsino],  
							nieid=$_REQUEST[nensino],  
							paequantprevista=$qtdprevista ";
			}
			if( $atendidos ){
				if( $where != '' ){
					$where2 = ",paequantatendida=".$atendidos;
				}else{
					$where2 = " paequantatendida=".$atendidos;
				}
			}
			
			$sql = "UPDATE pse.pseanoescola set  
					{$where}
					{$where2}
					WHERE 
					paeid = $_REQUEST[paeid]";
			$db->executar($sql);
			$db->commit();
			echo 1;
		}
		else {
			$sql = "select count(*) from pse.pseanoescola
					where paeanoreferencia=$_REQUEST[ano] and esfid=$_REQUEST[idesf] and 
						  moeid=$_REQUEST[modalidadeEnsino] and nieid=$_REQUEST[nensino]";
			$quant = $db->pegaum($sql);
			if($quant==0){
				if( $atendidos ){
					$at = ", ".$atendidos;
					$campo = ", paequantatendida";
				}
				$sql = "INSERT INTO pse.pseanoescola
						(paeanoreferencia, esfid, moeid, nieid, paequantprevista {$campo}) 
						VALUES 
						($_REQUEST[ano], $_REQUEST[idesf], $_REQUEST[modalidadeEnsino], $_REQUEST[nensino], $qtdprevista {$at})";
				$db->executar($sql);
				$db->commit();
				echo 1;
			}
			else
				echo "Dados já informados!";
		}
}

function alteraAtendimento($id){
	global $db;
			$sql = "select pae.paeid, pae.esfid, pae.moeid, pae.nieid, pae.paequantprevista, pae.paequantatendida, esf.espid
					from pse.pseanoescola pae
					inner join pse.equipesaudefamilia esf ON esf.esfid = pae.esfid
					where pae.paeid=$id";
			$dados = $db->pegalinha($sql);

			echo $dados['paeid']."|".$dados['esfid']."|".$dados['moeid']."|".$dados['nieid']."|".$dados['paequantprevista']."|".$dados['paequantatendida']."|".$dados['espid'];
}

function excluiAtendimento($id){
	global $db;
			$sql = "delete from pse.pseanoescola
					where paeid=$id";
			$db->executar($sql);
			$db->commit();
			echo 1;
}

function anoReferencia($espid){
	global $db;
	$sql = "SELECT espparticipapse2009 FROM pse.escolapse
			WHERE espid = $espid";
	$option = $db->pegaum($sql);
	$option =($option=='t'?'<option value="2009">2009</option>':'');
	echo "	<select name='anoref' id='anoref' size='1' onchange='pesquisaQD(this.value);regras(this.value);'>
    			<option value=''>Selecione...</option>
    			$option
    			<option value='2010'>2010</option>
    			<option value='2011'>2011</option>
    		</select>
    		 <img border='0' src='../imagens/obrig.gif'/>";
}

function Cabecalho($entid,$ler,$pflcod = null){
	global $db;
		if($ler==2){
			$sql = "SELECT m.estuf, m.mundescricao,e.muncodcapital
					FROM territorios.estado e
					INNER JOIN
					territorios.municipio m on m.estuf=e.estuf
					WHERE m.muncod = '$entid'";
			$dados = $db->pegalinha($sql);
			if($pflcod == 'e'){ 
				$secretaria = 'Estadual';
			} else {
				$secretaria = 'Municipal';
			}
			if( $_SESSION['exercicio'] == 2009 ){
				$portaria = "Portaria 2.931 de 4 de dezembro de 2008";
			} elseif( $_SESSION['exercicio'] == 2010 ){
				$portaria = "Portaria 1.537 de 15 de junho de 2010";
			}
			

		echo "		
			<table align=\"center\" class=\"Tabela\" cellpadding=\"2\" cellspacing=\"1\">
					 <tbody>
						<tr>
							<td width=\"100\" class=\"SubTituloEsquerda\"></td>
							<td width=\"90%\" class=\"SubTituloEsquerda\"><font size='2'>Secretaria $secretaria de Educação</font></td>
						</tr>
						<tr>
							<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloDireita\">UF:</td>
							<td width=\"90%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">".(($dados['estuf']==NULL)?'':$dados['estuf'])."</td>
						</tr>
						<tr>
							<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloDireita\">Município:</td>
							<td width=\"90%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">".(($dados['mundescricao']==NULL)?'':$dados['mundescricao'])."</td>
						</tr>
						<tr>
							<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloDireita\">Portaria:</td>
							<td width=\"90%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">".$portaria."</td>
						</tr>";
		}
		else {
		$sql ="	SELECT ent.entid,ent.entcodent, ent.entnome, ende.estuf, mun.mundescricao,ende.endlog||', '||ende.endcom||' - '||ende.endbai as endereco, professor.entid as identf, professor.entnome as dir  
				FROM entidade.entidade ent				
				LEFT JOIN 
					entidade.funcaoentidade feEscola on feEscola.entid = ent.entid
				LEFT JOIN 
					entidade.funentassoc assocprofessores on assocprofessores.entid = ent.entid
				LEFT JOIN 
					entidade.funcaoentidade feProfessor on feProfessor.fueid = assocprofessores.fueid	
				LEFT JOIN 
					entidade.entidade professor on professor.entid = feProfessor.entid
				LEFT JOIN
					entidade.endereco ende ON ende.entid = ent.entid
				LEFT JOIN
					entidade.entidadedetalhe entd ON entd.entid = ent.entid		
				INNER JOIN
					territorios.municipio mun ON mun.muncod = ende.muncod
				INNER JOIN
					territorios.estado est ON est.estuf = mun.estuf
				WHERE	ent.entstatus='A' AND 
						feEscola.funid in (3,4)
						and ent.entid = '$entid'";
			
		$dados = $db->pegalinha($sql);
		
		echo "		
			<table align=\"center\" class=\"Tabela\" cellpadding=\"2\" cellspacing=\"1\">
					 <tbody>
						<tr>
							<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloDireita\">Código INEP:</td>
							<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">".(($dados['entcodent']==NULL)?'':$dados['entcodent'])."</td>
						</tr>
						<tr>
							<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloDireita\">Nome da Escola:</td>
							<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">".(($dados['entnome']==NULL)?'':$dados['entnome'])."</td>
						</tr>
						<tr>
							<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloDireita\">Endereço da Escola:</td>
							<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">".(($dados['endereco']==NULL)?'':$dados['endereco'])."</td>
						</tr>
						<tr>
							<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloDireita\">UF:</td>
							<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">".(($dados['estuf']==NULL)?'':$dados['estuf'])."</td>
						</tr>
						<tr>
							<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloDireita\">Município:</td>
							<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">".(($dados['mundescricao']==NULL)?'':$dados['mundescricao'])."</td>
						</tr>
						<!--
						<tr>
							<td width=\"100\" style=\"text-align: right;\" class=\"SubTituloDireita\">Diretor da Escola:</td>
							<td width=\"80%\" style=\"background: rgb(238, 238, 238) none repeat scroll 0% 0%; text-align: left; -moz-background-clip: -moz-initial; -moz-background-origin: -moz-initial; -moz-background-inline-policy: -moz-initial;\" class=\"SubTituloDireita\">".(($dados['dir']==NULL)?'':$dados['dir'])."</td>
						</tr>
						-->
						</tbody>
			</table>";
		}
		
				
		
}

/*
 * Monta Arvore 
 */
function montaArvore($ler,$id, $empid = null){
	global $db;
	$usucpf = $_SESSION['usucpf'];
	$pflcod = pegaPerfil($usucpf);
	
	$tree .= "<div id=\"arvore\"></div>";

	$tree .= "<script type=\"text/javascript\">
				
				arvoreP = new dTree( 'arvoreP' );
				arvoreP.config.folderLinks = true;
				arvoreP.config.useIcons = true;
				arvoreP.config.useCookies = true;\n";
	
	// ARVORE PARA ESCOLAS			
	if($ler==1){
	// Verifica preenchimento - Identificação da Escola
	$sql = "select count(*) as qd, espparticipapse2009 as tipo from pse.escolapse
			  where entid = $id and (espparticipapse2009 = 't' OR espparticipapse2009 = 'f') and espano = ".$_SESSION['exercicio']."
			  group by espparticipapse2009";
	$dados = $db->pegalinha($sql);
	$img1 = ($dados['qd'] == 0 ? '../imagens/atencao.png':'../imagens/check_p.gif');
	$termo = $dados['qd'];
	
	// Verifica preenchimento - ESF - Equipe(s) Saúde Família
	$sql = "select count(*) from pse.equipesaudefamilia esf
			  inner join
			    pse.escolapse esp ON esp.espid=esf.espid
			  where esp.entid=$id and esp.espano = ".$_SESSION['exercicio'];
	$img12 = $db->pegaUm($sql);
	$termo = ($termo==0?0:$img12);
	$img12 = ($img12 == 0 ? '../imagens/atencao.png':'../imagens/check_p.gif');
	
	
	// Verifica preenchimento - Educador(es) e Profissional(is) de Saúde de Referência do PSE
	$sql = "select count(*) from pse.educadoreferencia edr
			  inner join
			    pse.escolapse esp ON esp.espid=edr.espid
			  where esp.entid=$id and esp.espano = ".$_SESSION['exercicio'];
	$img13 = $db->pegaUm($sql);
	$termo = ($termo==0?0:$img13);
	$img13 = ($img13 == 0 ? '../imagens/atencao.png' : '../imagens/check_p.gif');
	
	//Verifica portaria
	$sql = "select MIN(espano) from pse.escolapse where entid = {$id} and espano is not null";
	$anoPort = $db->pegaUm( $sql );
	$anoPort2 = $anoPort + 1;
	$anoPort3 = $anoPort + 2;

	// Verifica preenchimento - Quantitativo(s) de estudantes, modalidades, etc...
	$sql = "select 
			(select count(*) from pse.pseanoescola pse 
			inner join
				pse.equipesaudefamilia esf ON esf.esfid=pse.esfid
			inner join
				pse.escolapse esp ON esp.espid=esf.espid
			where esp.entid=$id and pse.paeanoreferencia={$anoPort}) AS ano1,
			(select count(*) from pse.pseanoescola pse 
			inner join
				pse.equipesaudefamilia esf ON esf.esfid=pse.esfid
			inner join
				pse.escolapse esp ON esp.espid=esf.espid
			where esp.entid=$id and pse.paeanoreferencia={$anoPort2}) as ano2,
			(select count(*) from pse.pseanoescola pse 
			inner join
				pse.equipesaudefamilia esf ON esf.esfid=pse.esfid
			inner join
				pse.escolapse esp ON esp.espid=esf.espid
			where esp.entid=$id and pse.paeanoreferencia={$anoPort3}) as ano3";
		$img2 = $db->pegalinha($sql);

		if( $_SESSION['exercicio'] == $anoPort ){
			if($img2['ano1']==0 ||$img2['ano2']==0 ||$img2['ano3']==0)
				$completo = 0;
			else
				$completo = 1;
		} elseif( $_SESSION['exercicio'] == $anoPort2 ){
			if($img2['ano2']==0 ||$img2['ano3']==0)
				$completo = 0;
			else
				$completo = 1;
		} elseif( $_SESSION['exercicio'] == $anoPort3 ){
			if($img2['ano3']==0)
				$completo = 0;
			else
				$completo = 1;
		}
		
		$img2 = $completo;
		$termo = ($termo==0?0:$img2);
		$img2 = ($img2 == 0?'../imagens/atencao.png':'../imagens/check_p.gif');				
		
	
	// Verifica preenchimento - Atores que integram a equipe do PSE
	$sql = "select count(*) from pse.atorequipe at
			  inner join
			    pse.escolapse esp ON esp.espid=at.espid
			  where esp.entid=$id and esp.espano = ".$_SESSION['exercicio'];
	$img3 = $db->pegaUm($sql);
	$termo = ($termo==0?0:$img3);
	$img3 = ($img3==0 ? '../imagens/atencao.png':'../imagens/check_p.gif');
	$impumg = '../imagens/print.png';
	
						
	$tree .= "	arvoreP.add( 0, -1, 'Escola' );
				arvoreP.add( 1, 0, ' 1. Identificacao da Escola', 'javascript: abrirJanela(\'CadastroEscola\');', '', '', '$img1' );
				arvoreP.add( 2, 0, '  1.2 ESF - Equipe(s) Saúde Família/ Unidade Básica de Saúde', 'javascript: abrirJanela(\'EquipeSaudeFamilia\');', '', '', '$img12' );
				arvoreP.add( 3, 0, '  1.3 Educador(es) e Profissional(is) de Saúde de Referência do PSE', 'javascript: abrirJanela(\'EducadorReferencia\');', '', '', '$img13' );
				arvoreP.add( 4, 0, ' 2. Quantitativo(s) de estudantes, modalidades, e seus respectivos níveis de ensino', 'javascript: abrirJanela(\'AtendimentoESF\');', '', '', '$img2' );
				arvoreP.add( 5, 0, ' 3. Atores que integram a equipe do PSE na Escola', 'javascript: abrirJanela(\'AtoresPSE\');', '', '', '$img3' );
				arvoreP.add( 6, 0, ' 4. Imprimir relatório da Escola', 'javascript: abrirJanela2(\'relEscola\');', '', '', '$impumg' );
			";
				
	
	//Retirado por solicitação, de acordo com o Juvenal, do jurídico,		
//	if($termo>0 && ($pflcod == MEC || $pflcod == ESCOLA_ESTADUAL || $pflcod == ESCOLA_MUNICIPAL || $pflcod == SUPER_USUARIO))
//		$tree .= "arvoreP.add( 7, 0, ' 5. Termo de Compromisso', 'javascript: abrirtermo(\'termoEscola\');', '', '', '../imagens/report.gif' );";


	}
	
	
	//ARVORE PARA SECRETARIAS
	else if($ler==2){ 
	
	$_SESSION['pse']['flagmun'] = $empid;	
		
	// Verifica preenchimento - Identificação da Secretaria
	$sql = "select semnomesecretariosaude,semendsecretariasaude,
			semnomerepresecretariasaude,sememailrepresecretariasaude,
			semtelefonerepresecretariasaude,semcargofuncaorepresecretariasa,
			semnomesecretarioeducacao,semendsecretariaeducacao,
			semnomerepresecretariaeducacao,sememailrepresecretariaeducacao,
			semtelefonerepresecretariaeduca,semcargofuncaorepresecretariaed
			from pse.estadomunicipiopse a
			inner join
				pse.secretariaestmun b on b.empid=a.empid
			where a.muncod='$id' AND a.empflagestmun = '$empid' AND a.empano = ".$_SESSION['exercicio'];

	$dados = $db->pegaLinha($sql);
	if ($dados['semnomesecretariosaude']==''||
		$dados['semendsecretariasaude']==''||
		$dados['semnomerepresecretariasaude']==''||
		$dados['sememailrepresecretariasaude']==''||
		$dados['semtelefonerepresecretariasaude']==''||
		$dados['semcargofuncaorepresecretariasa']==''||
		$dados['semnomesecretarioeducacao']==''||
		$dados['semendsecretariaeducacao']==''||
		$dados['semnomerepresecretariaeducacao']==''||
		$dados['sememailrepresecretariaeducacao']==''||
		$dados['semtelefonerepresecretariaeduca']==''||
		$dados['semcargofuncaorepresecretariaed']=='')
		
		$img11=0;
	else
		$img11=1;
	
	$termo = $img11;
	
	$img11 = ($img11 == 0 ? '../imagens/atencao.png':'../imagens/check_p.gif');

		
	// Verifica preenchimento - Representantes, Grupo Gestor, Projeto e Gestão
	$perInicio		= 1;
	$perFim			= 17;
	$representante	= array('1');
	$gGestor		= array('2','3','4','5');
	$projeto		= array('6','7','8','9');
	$gestao			= array('10','11','12','13');
	$educCapacitacao= array('14','15','16','17');
			
	for($i=$perInicio;$i<=$perFim;$i++){
		$sql = "select p.perid, p.perflagboleana, p.perflagitem, p.perflagmultiescolha, r.resid, r.resflagresposta
				from pse.pergunta p
				inner join
					pse.resposta r on r.perid=p.perid
				inner join
					pse.estadomunicipiopse e on e.empid=r.empid	
				where
					p.perid = $i and
					e.muncod='$id' and
					e.empflagestmun = '$empid' and
					e.empano = ".$_SESSION['exercicio'];
		$dados = $db->pegaLinha($sql);

		if($dados=='') {
			$imagem = 0;
			$termo = ($termo==0?0:$imagem);
		}
		else {	
			if($dados['perflagboleana']=='s'){
				if($dados['resflagresposta']=='n'){
					$imagem = 1;
					$termo = ($termo==0?0:$imagem);
				}
				else {
					if($dados['perflagitem']=='s'){
						$sql = "select count(*) 
								from pse.pergunta p
								inner join
									pse.resposta r on r.perid=p.perid
								inner join
									pse.estadomunicipiopse e on e.empid=r.empid
								inner join
									pse.itemselecionado s on s.resid=r.resid
								where p.perid =$i and
									  e.muncod='$id' and
									  r.resid=$dados[resid] and
									  e.empflagestmun = '$empid' and
									  e.empano = ".$_SESSION['exercicio'];
						$dados = $db->pegaUm($sql);

						if($dados>0){
							$imagem=1;
							$termo = ($termo==0?0:$imagem);
						}
						else{
							$imagem=0;
							$termo = ($termo==0?0:$imagem);
						}
					}
				}
			}
			else {
				if($dados['perflagitem']=='s'){
					$sql = "select count(*) 
							from pse.pergunta p
							inner join
								pse.resposta r on r.perid=p.perid
							inner join
								pse.estadomunicipiopse e on e.empid=r.empid
							inner join
								pse.itemselecionado s on s.resid=r.resid
							where p.perid =$i and
								  e.muncod='$id' and
								  r.resid=$dados[resid] and
								  e.empflagestmun = '$empid' and
								  e.empano = ".$_SESSION['exercicio'];
					$dados = $db->pegaUm($sql);
					if($dados>0){
						$imagem=1;
						$termo = ($termo==0?0:$imagem);
					}
					else{
						$imagem=0;
						$termo = ($termo==0?0:$imagem);
					}
				}	
				
			}
			
		}

		if (in_array($i, $representante))
			$img12 = ($imagem == 0 ? '../imagens/atencao.png':'../imagens/check_p.gif');
   		else if (in_array($i, $gGestor))
			$img15 = ($imagem == 0 ? '../imagens/atencao.png':'../imagens/check_p.gif');
		else if (in_array($i, $projeto))
			$img16 = ($termo == 0 ? '../imagens/atencao.png':'../imagens/check_p.gif');
		else if (in_array($i, $gestao))
			$img17 = ($imagem == 0 ? '../imagens/atencao.png':'../imagens/check_p.gif');
		else if (in_array($i, $educCapacitacao))
			$img22 = ($imagem == 0 ? '../imagens/atencao.png':'../imagens/check_p.gif');
		
	}

	//Verifica portaria
	$sql = "SELECT (porano + 1) FROM pse.portariapse WHERE pormunicipio = '".$id."'";
	$anoPort = $db->pegaUm( $sql );
//	$anoPort++;
	$anos = array();
	
	if( $_SESSION['exercicio'] == $anoPort ){
		array_push( $anos, $anoPort );
		array_push( $anos, $anoPort + 1 );
		array_push( $anos, $anoPort + 2 );
	} elseif( $_SESSION['exercicio'] == $anoPort + 1 ) {
		array_push( $anos, $anoPort + 1 );
		array_push( $anos, $anoPort + 2 );
	} else {
		array_push( $anos, $anoPort + 2 );
	}
	
	foreach( $anos as $ano => $v ){
		if( $v >= 2012 ){
			unset( $anos[$ano] );
		}
	}
	
	// Verifica preenchimento - PSE
	/*
	$sql = "select
			(select empparticipapse2009 from pse.estadomunicipiopse
			 where muncod='$id' and empflagestmun = '$empid' and empano = ".$_SESSION['exercicio'].") AS flag,
			(select count(*) from pse.pseanoescolamun pse 
			inner join
				pse.estadomunicipiopse est ON est.empid=pse.empid
			where est.muncod='$id' and pse.pamanoreferencia={$anoPort} and est.empflagestmun = '$empid') AS ano2009,
			(select count(*) from pse.pseanoescolamun pse 
			inner join
				pse.estadomunicipiopse est ON est.empid=pse.empid
			where est.muncod='$id' and pse.pamanoreferencia={$anoPort2} and est.empflagestmun = '$empid') AS ano2010,
			(select count(*) from pse.pseanoescolamun pse 
			inner join
				pse.estadomunicipiopse est ON est.empid=pse.empid
			where est.muncod='$id' and pse.pamanoreferencia={$anoPort3} and est.empflagestmun = '$empid') AS ano2011";
		$img13 = $db->pegalinha($sql);
		ver($sql);
		if($img13['ano2009']==0 ||$img13['ano2010']==0 ||$img13['ano2011']==0)
			$completo = 0;
		else
			$completo = 1;
		
*/
	
	$sql = "SELECT DISTINCT 
				pse.pamanoreferencia
			FROM 
				pse.pseanoescolamun pse 
			inner join
				pse.estadomunicipiopse est ON est.empid=pse.empid
			WHERE
				est.muncod='$id'"; 
			//	AND est.empano = ".$_SESSION['exercicio'];
	
	$resultado = $db->carregarColuna( $sql );
//ver($sql);
	if( array_diff( $anos, $resultado )){
		$completo = 0;
	} else {
		$completo = 1;
	}
	
	$img13 = $completo;
	$termo = ($termo==0?0:$img13);
	$img13 = ($img13 == 0?'../imagens/atencao.png':'../imagens/check_p.gif');	
	
	// Verifica preenchimento - Materiais clínicos e didáticos
	$sql = "select
			(select iteid
			from pse.itemselecionado i
			inner join
				pse.resposta r on r.resid = i.resid
			inner join
				pse.pergunta p on p.perid=r.perid
			inner join
				pse.estadomunicipiopse e on e.empid=r.empid	
			where p.perid = 19 and e.muncod='$id' and e.empflagestmun = '$empid' and e.empano = ".$_SESSION['exercicio'].") as check,
			(select count(*)
			from pse.secrekitcompleto k
			inner join
				pse.estadomunicipiopse e on e.empid=k.empid	
			where e.muncod='$id' and e.empflagestmun = '$empid' and e.empano = ".$_SESSION['exercicio'].") as kits";
		$img14 = $db->pegalinha($sql);

		if($img14['check']<>55){
			if($img14['kits']==0)
				$completo = 0;
			else
				$completo = 1;
		}
		else 
				$completo = 1;
		
		$img14 = $completo;
		$termo = ($termo==0?0:$img14);
		$img14 = ($img14 == 0?'../imagens/atencao.png':'../imagens/check_p.gif');			

	//COMPONENTES /ATIVIDADES
	$componentes = array(1=>array(2,3,4),
						 2=>array(5),
						 3=>array(6),
						 4=>array(8)
					); 

	for($i=1;$i<=count($componentes);$i++){
		foreach($componentes[$i] as $atividade=>$valor){
				$sql =	"select count(*)
						from pse.quantidadeescola e
						left join
							pse.atividade a on a.atvid=e.atvid
						inner join
							pse.componente c on c.copid=a.copid
						inner join
							pse.estadomunicipiopse d on d.empid=e.empid	
						where d.muncod = '$id' and c.copid = $valor and d.empflagestmun = '$empid' and d.empano = ".$_SESSION['exercicio'];
				$dados = $db->pegaUm($sql);
				if( $i <> 4 ){
					$termo = ($termo==0?0:$dados);
				}
				if (in_array($valor, $componentes[1]))
					$conta1 = ($conta1 + ($dados==0?0:1));
		   		else if (in_array($valor, $componentes[2]))
					$conta2 = ($conta2 + ($dados==0?0:1));
				else if (in_array($valor, $componentes[3]))
					$conta3 = ($conta3 + ($dados==0?0:1));
				else if (in_array($valor, $componentes[4]))
					$conta4 = ($conta4 + ($dados==0?0:1));
					
				if($i==3){	
					$sql =	"select count(*)
							from pse.itemselecionado i
							inner join
								pse.resposta r on r.resid=i.resid
							inner join
								pse.pergunta p on p.perid = r.perid
							inner join
								pse.estadomunicipiopse e ON e.empid=r.empid
							where e.muncod='$id' and r.perid=18 and e.empflagestmun = '$empid' and e.empano = ".$_SESSION['exercicio'];
					$perg18 = $db->pegaUm($sql);
					$conta3a = ($perg18==0?0:1);
					$termo = ($termo==0?0:$perg18);
				}
				if( $i==4 ){
					$sql =	"select count(*)
							from pse.resposta r
							inner join
								pse.estadomunicipiopse e ON e.empid=r.empid
							where e.muncod='$id' and r.perid IN (20, 21) and e.empflagestmun = '$empid' and e.empano = ".$_SESSION['exercicio'];
					$perg2021 = $db->pegaUm($sql);
					$conta4 = ($perg2021<2?0:1);
					if($conta4 <> 0){
						$termo = ($termo==0?0:$perg2021);
					} else {
						$termo = 0;
					}
				}
		}
		if($i<>3){
			${"imgcomp$i"} = (${"conta$i"} == count($componentes[$i])?'../imagens/check_p.gif':'../imagens/atencao.png');
		}
		else {
			if ($conta3==1 && $conta3a==1)
				$conta3=1;
			else
				$conta3=0;
				
			${"imgcomp$i"} = (${"conta$i"}==1?'../imagens/check_p.gif':'../imagens/atencao.png');
		}
		
	}
	
			
	$tree .= "	arvoreP.add( 80, -1, 'Secretaria' );
				arvoreP.add( 81, 80, ' 1-Identificação', 'javascript: abrirJanela(\'identificacaoEstadoMunicipio\');', '', '', '$img11'  );
				arvoreP.add( 82, 80, ' 2-Representantes do PSE', 'javascript: abrirJanela(\'representante\');', '', '', '$img12'  );
				arvoreP.add( 83, 80, ' 3-PSE 2009/2010/2011', 'javascript: abrirJanela(\'pse2009\');', '', '', '$img13'  );
				arvoreP.add( 84, 80, ' 4-Materiais Clínicos e Didáticos', 'javascript: abrirJanela(\'materialClinicoDidatico\');', '', '', '$img14'  );
				arvoreP.add( 85, 80, ' 5-Grupo gestor do projeto saúde e prevenção na escola - PSE/SPE', 'javascript: abrirJanela(\'grupoGestor\');', '', '', '$img15'  );
				arvoreP.add( 86, 80, ' 6-Projeto', 'javascript: abrirJanela(\'projetoEstadualMunicipal\');', '', '', '$img16'  );
				arvoreP.add( 87, 80, ' 7-Gestão', 'javascript: abrirJanela(\'gestao\');', '', '', '$img17'  );
				arvoreP.add( 88, 80, ' 8-Componente I/ Ações', 'javascript: abrirJanela(\'componente01\');', '', '', '$imgcomp1'  );
				arvoreP.add( 89, 80, ' 9-Componente II/ Ações', 'javascript: abrirJanela(\'componente02\');', '', '', '$imgcomp2'  );
				arvoreP.add( 90, 80, ' 10-Componente III/ Ações', 'javascript: abrirJanela(\'componente03\');', '', '', '$imgcomp3'  );
				arvoreP.add( 91, 80, ' 11-Componente IV / Ações', 'javascript: abrirJanela(\'componente04\');', '', '', '$imgcomp4'  );
			";
	
	//mudar termo para maior que 0 (>0)		
	if($termo>0 && ($pflcod == MEC || $pflcod == SECRETARIA_ESTADUAL || $pflcod == SECRETARIA_MUNICIPAL || $pflcod == SUPER_USUARIO))
		$tree .= "arvoreP.add( 93, 80, ' Termo de Adesão', 'javascript: abrirtermo(\'termoSecretaria\');', '', '', '../imagens/report.gif' );";
				
	
	}
	
	//ARVORE PARA UNIDADE LOCAL INTEGRADA
	else {
		
		//Troca das imagens nas perguntas
		$sql = "select espid from pse.escolapse
			    where entid = {$id} and espano = ".$_SESSION['exercicio']; //and (espparticipapse2009 = 't' OR espparticipapse2009 = 'f')";
		$espid = $db->pegaUm($sql);
		
		//Criando array com as perguntas que serão utilizadas
		//As perguntas são de 1 a 42
		$ultimaPergunta = 42;
		$arrperguntas = array();
		for($i=1;$i<=$ultimaPergunta;$i++){
			$arrperguntas[$i] = $i;
		}
		
		//Laço para verificar a situação da pergunta
		/*foreach($arrperguntas as $pergunta){
			$imagem = '';
			$sql = "SELECT  p.pulid,p.pulboleana,p.pulflagitem,p.pulflagmultiescolha,r.rulid,r.paqid,r.rulflagresposta
					FROM pse.pulpergunta p
					INNER JOIN
						pse.rulresposta r ON r.pulid = p.pulid
					WHERE r.espid = $espid AND p.pulid = $pergunta";
			$dados = $db->pegaLinha($sql);

			//Verifica se a pergunta existe com sua resposta
			//Não
			if(!$dados) 
				$imagem = 0;
			//Sim	
			else {
				//pergunta é boleana	
				if($dados['pulboleana']=='s'){
					//verifica se tem resposta
					$imagem = ($dados['rulflagresposta']==''?0:1);
				}
				//Verifica se tem itens
				if($dados['pulflagitem']=='s'){
					$sql = "SELECT count(*)  
							FROM pse.isuitemselecionado
							WHERE rulid = $dados[rulid]";
					$itens = ($db->pegaUm($sql)==0?0:1);
					$imagem = $itens;
				}
			}
			
			${"img_$pergunta"} = $imagem;
			echo "perg: ".$pergunta." - ".${"img_$pergunta"}."<br>";
		}*/
		
		//Laço para verificar a situação da pergunta
		foreach($arrperguntas as $pergunta){
			$imagem = 0;
			$sql = "SELECT  p.pulid,p.pulboleana,p.pulflagitem,p.pulflagmultiescolha,r.rulid,r.paqid,r.rulflagresposta,
					(
						SELECT count(*) FROM pse.isuitemselecionado i
						INNER JOIN
							pse.rulresposta r ON r.pulid = p.pulid and r.rulid = i.rulid
						WHERE r.espid = $espid AND p.pulid = $pergunta
					
					) as itens
					FROM pse.pulpergunta p
					INNER JOIN
						pse.rulresposta r ON r.pulid = p.pulid
					WHERE r.espid = $espid AND p.pulid = $pergunta";
					if( $espid == '' || empty($espid)){
						print "<script>"
							. "    alert('Você não pode acessar essa área até fazer o cadastro em Identificação!');"
							. "    history.back(-1);"
							. "</script>";
						
						die;
			   		}
			$dados = $db->pegaLinha($sql);
//dbg($dados);
			//Verifica se a pergunta existe com sua resposta
			//Não
			if(!$dados) 
				$imagem = 0;
			//Sim	
			else {
				if($dados['pulflagitem']=='s'){
					if($dados['itens']>0)
						$imagem = 1;
					else {
						if($dados['rulflagresposta']=='n')
							$imagem = 1;
						else
							$imagem = 0;
					}
				}
				else {
					$imagem = 1;
					if( $dados['rulflagresposta']=='n' ){
						$item = 1;
					} else {
						$item = 0;
					}
				}

			}

			${"img_$pergunta"} = $imagem;
			${"semitem_$pergunta"} = $item;
			//echo "perg: ".$pergunta." - ".${"img_$pergunta"}."<br>";
		}
		
		$pergPage = array(
							1=>array(1),		2=>array(2),		3=>array(3),
//							4=>array(4,5),		5=>array(6,8),		6=>array(10,11),
							4=>array(4),		5=>array(6,8),		6=>array(10,11),
							7=>array(12,17),	8=>array(18),		9=>array(13,14),
							10=>array(15),		11=>array(16),		12=>array(19),
							13=>array(20),		14=>array(21),		15=>array(22,23),
							16=>array(24),		17=>array(25),		18=>array(26),
							19=>array(27),		20=>array(28),		21=>array(29),
							22=>array(30),		23=>array(31,32,33,34,35,36,37,38,39,40,41,42)
						 );
					 
		$pergunta8 = 0;
		foreach ($pergPage as $k => $v) {
			$verifica = 0;
			$conta = 0;
			$total = 0;
			if( $k == 23 ){
				for($z=0;$z<=count($v)-1;$z++){
					$sql = "SELECT
								r.rulflagresposta,
								(
									SELECT count(*) FROM pse.isuitemselecionado i
									INNER JOIN
										pse.rulresposta r ON r.pulid = p.pulid and r.rulid = i.rulid
									WHERE r.espid = $espid AND p.pulid = $v[$z]
								
								) as itens
							FROM
								pse.pulpergunta p
							INNER JOIN
								pse.rulresposta r ON r.pulid = p.pulid
							WHERE
								r.espid = $espid AND p.pulid = $v[$z]";
					$dados = $db->pegaLinha($sql);
					if( $v[$z] == 31 || $v[$z] == 32 || $v[$z] == 34 || $v[$z] == 39 || $v[$z] == 40 || $v[$z] == 41 ){
						if( !$dados['rulflagresposta'] ){
							$verifica = 1;
						}
					} else {
						if( $dados['itens'] == 0 ){
							$verifica = 1;
						}
					}
				}
				${"imgP23"} = ( $verifica == 0 ? '../imagens/check_p.gif' : '../imagens/atencao.png');
			} elseif( $k == 7 ){
				for($z=0;$z<=count($v)-1;$z++){
					$sql = "SELECT
								r.rulflagresposta
							FROM
								pse.pulpergunta p
							INNER JOIN
								pse.rulresposta r ON r.pulid = p.pulid
							WHERE
								r.espid = $espid AND p.pulid = $v[$z]";
					$flag = $db->pegaUm($sql);
					if( $flag ){
						if( $flag == 's' ){
							$sql = "SELECT
										(
											SELECT count(*) FROM pse.isuitemselecionado i
											INNER JOIN
												pse.rulresposta r ON r.pulid = p.pulid and r.rulid = i.rulid
											WHERE r.espid = $espid AND p.pulid = $v[$z]
										
										) as itens
									FROM
										pse.pulpergunta p
									INNER JOIN
										pse.rulresposta r ON r.pulid = p.pulid
									WHERE
										r.espid = $espid AND p.pulid = $v[$z]";
							
							if( $espid == '' || empty($espid)){
									print "<script>"
										. "    alert('Você não pode acessar essa área até fazer o cadastro em Identificação!');"
										. "    history.back(-1);"
										. "</script>";
									
									die;
						    }
						    
							$it = $db->pegaUm($sql);
							if( $it ){
								if( $it == 0 ){
									$verifica = 1;
								}
							}
						} else {
							$pergunta8 = 1;
						}
					} else {
						$verifica = 1;
					}
				}
				${"imgP7"} = ( $verifica == 0 ? '../imagens/check_p.gif' : '../imagens/atencao.png');
			} elseif( $k == 6 ){
				for($z=0;$z<=count($v)-1;$z++){
					$sql = "SELECT
								r.rulflagresposta
							FROM
								pse.pulpergunta p
							INNER JOIN
								pse.rulresposta r ON r.pulid = p.pulid
							WHERE
								r.espid = $espid AND p.pulid = $v[$z]";
					$flag = $db->pegaUm($sql);
					if( $flag ){
						if( $flag == 's' ){
							$sql = "SELECT
										(
											SELECT count(*) FROM pse.isuitemselecionado i
											INNER JOIN
												pse.rulresposta r ON r.pulid = p.pulid and r.rulid = i.rulid
											WHERE r.espid = $espid AND p.pulid = $v[$z]
										
										) as itens
									FROM
										pse.pulpergunta p
									INNER JOIN
										pse.rulresposta r ON r.pulid = p.pulid
									WHERE
										r.espid = $espid AND p.pulid = $v[$z]";
							$it = $db->pegaUm($sql);
							if( $it ){
								if( $it == 0 ){
									$verifica = 1;
								}
							}
						} else {
							$pergunta10 = 1;
						}
					} else {
						$verifica = 1;
					}
				}
				${"imgP6"} = ( $verifica == 0 ? '../imagens/check_p.gif' : '../imagens/atencao.png');
			} else {
				
				for($z=0;$z<=count($v)-1;$z++){
					$conta = ($conta + ${"img_$v[$z]"});
					${"imgP$k"} = $conta;
					${"totalP$k"} = count($v);
					if( ${"semitem_$v[$z]"} && ${"img_$v[$z]"} == 1 ){
						$conta = ${"totalP$k"};
					}					
				}
				$total = ${"totalP$k"};
				//echo "page: ".$k." - total img: ".${"imgP$k"}."total: ".$total."<br>";
				//echo "array: ".$k." - ".${"imgP$k"}."<br>";
				${"imgP$k"} = ($conta == $total ? '../imagens/check_p.gif' : '../imagens/atencao.png');
				if( $k == 8 && $pergunta8 == 1 ){
					${"imgP$k"} = '../imagens/check_p.gif';
				}
			}
		}

		$tree .= "
				arvoreP.add( 20, -1, 'Unidade Local Integrada' );
					arvoreP.add( 21, 20, 'Gestão' );
						arvoreP.add( 22, 21, 'Atores que participaram da execução da Agenda de Educação e Saúde', 'javascript: abrirJanela(\'gestao01\');', '', '', '$imgP1' );		
						arvoreP.add( 23, 21, 'Parcerias para implementação do programa', 'javascript: abrirJanela(\'gestao02\');', '', '', '$imgP2' );
						arvoreP.add( 24, 21, 'Informações para planejamento', 'javascript: abrirJanela(\'gestao03\');', '', '', '$imgP3' );
						arvoreP.add( 25, 21, 'Frequência de comunicação', 'javascript: abrirJanela(\'gestao04\');', '', '', '$imgP4' );
						arvoreP.add( 26, 21, 'Agenda Educação e Saúde', 'javascript: abrirJanela(\'gestao05\');', '', '', '$imgP5' );
					arvoreP.add( 27, 20, 'Componente I' );
						arvoreP.add( 28, 27, 'Material enviado pelo MEC', 'javascript: abrirJanela(\'compInecessidadeCarencia01\');', '', '', '$imgP6' );		
						arvoreP.add( 29, 27, 'Plano de atendimento', 'javascript: abrirJanela(\'compInecessidadeCarencia02\');', '', '', '$imgP7' );
						arvoreP.add( 30, 27, 'Avaliação Clínica e Psicosocial', 'javascript: abrirJanela(\'compInecessidadeCarencia03\');', '', '', '$imgP8' );
						arvoreP.add( 31, 27, 'Avaliação nutricional e Saúde Bucal', 'javascript: abrirJanela(\'compInecessidadeCarencia04\');', '', '', '$imgP9' );
						arvoreP.add( 32, 27, 'Estratégias planejadas para avaliaçãoclínica e psicosocial', 'javascript: abrirJanela(\'compInecessidadeCarencia05\');', '', '', '$imgP10' );
						arvoreP.add( 33, 27, 'Atores envolvidos na avaliação', 'javascript: abrirJanela(\'compInecessidadeCarencia06\');', '', '', '$imgP11' );
					arvoreP.add( 34, 20, 'Componente II' );
						arvoreP.add( 35, 34, 'Ações de segurança alimentar e Promoção da Alimentação Saudável', 'javascript: abrirJanela(\'compII01\');', '', '', '$imgP12' );		
						arvoreP.add( 40, 34, 'Estratégias realizadas para execução das ações de prevenção  e promoção de saúde', 'javascript: abrirJanela(\'compII06\');', '', '', '$imgP13' );		
						arvoreP.add( 41, 34, 'Atores envolvidos nas ações de prevenção e promoção da saúde na escola', 'javascript: abrirJanela(\'compII07\');', '', '', '$imgP14' );		
						arvoreP.add( 42, 34, 'Articulação com ESF / UBS', 'javascript: abrirJanela(\'compII08\');', '', '', '$imgP15' );				
					arvoreP.add( 43, 20, 'Componente III' );
						arvoreP.add( 44, 43, 'Ações / Atividades realizadas', 'javascript: abrirJanela(\'compIII01\');', '', '', '$imgP16' );
					arvoreP.add( 45, 20, 'Reconhecimento de Território' );
						arvoreP.add( 46, 45, 'Ações realizadas para intercâmbio', 'javascript: abrirJanela(\'reconhecimento01\');', '', '', '$imgP17' );		
						arvoreP.add( 47, 45, 'Participação da comunidade escolar e jovens', 'javascript: abrirJanela(\'reconhecimento02\');', '', '', '$imgP18' );
						arvoreP.add( 48, 45, 'Registros das experiências locais', 'javascript: abrirJanela(\'reconhecimento03\');', '', '', '$imgP19' );
						arvoreP.add( 49, 45, 'Ações do PSE inseridas no projeto político-Pedagogico', 'javascript: abrirJanela(\'reconhecimento04\');', '', '', '$imgP20' );
						arvoreP.add( 50, 45, 'Encaminhamento dos estudantes aos especialistas', 'javascript: abrirJanela(\'reconhecimento05\');', '', '', '$imgP21' );
						arvoreP.add( 51, 45, 'Esclarecimento aos pais', 'javascript: abrirJanela(\'reconhecimento06\');', '', '', '$imgP22' );
					arvoreP.add( 52, 20, 'Ambiente' );
						arvoreP.add( 53, 52, 'Ambiente Escolar', 'javascript: abrirJanela(\'ambienteEscolar\');', '', '', '$imgP23' );
			";
		
		
		
	}	
	
		

	$tree .= "  elemento = document.getElementById( 'arvore' );
			    elemento.innerHTML = arvoreP;
			  </script>";

	return $tree;
	
}

function navegacao($antes,$depois){
echo "
	<tr bgcolor=\"#cccccc\">
		<td colspan=\"2\">
			<table align=\"center\" class=\"Tabela\" cellpadding=\"2\" cellspacing=\"1\">
				<tbody>
					<tr>
						<td>
							<input type=\"button\" class=\"botao\" name=\"btanterior\" value=\"Anterior\" onclick=\"navega('$antes')\">
						</td>
						<td style=\"text-align: center\">
							<input type=\"button\" class=\"botao\" name=\"btvoltar\" value=\"Voltar\" onclick=\"window.location.href='/pse/pse.php?modulo=principal/cadastroEstadoMunicipioArvore&acao=A';\">
						</td>
						<td align=\"right\">
							<input type=\"button\" class=\"botao\" name=\"btproximo\" value=\"Próximo\" onclick=\"navega('$depois')\">
						</td> 
					</tr> 
				</tbody>
			</table>
		</td>		
	</tr>
		
";	
}

function itensPergunta($empid){
	global $db;
	$sql = "SELECT m.kitflagpedcli,sum(skcquantescola) as escola, sum(skcquantesf) as esf
			FROM pse.secrekitcompleto k
			INNER JOIN
			pse.kitpedagoclinico m on m.kitid = k.kitid
			where k.empid=$empid
			group by m.kitflagpedcli";	
	$totais = $db->carregar( $sql );
	if($totais<>''){
		foreach($totais as $totais){
			if($totais['kitflagpedcli']=='p'){
				$totalPESC = $totais['escola'];
				$totalPESF = $totais['esf'];
			}
			else{
				$totalCESC = $totais['escola'];
				$totalCESF = $totais['esf'];
			}
		}
	}
	
	$sql = "SELECT porano + 1 FROM pse.portariapse p INNER JOIN pse.estadomunicipiopse e ON p.pormunicipio = e.muncod WHERE e.empid = ".$empid;
	$ano = $db->pegaum( $sql );
	
	if( !($ano == $_SESSION['exercicio']) ){
		$where = "AND kitano = ".$_SESSION['exercicio'];
	} else {
		$where = "AND kitano <= ".$_SESSION['exercicio'];
	}
	
	$sql = "SELECT count(*)
			FROM pse.kitpedagoclinico
			WHERE kitflagpedcli='p' AND kitsituacao = '1'
			".$where;	
	$conta1 = $db->pegaum( $sql );
	$conta2 = $db->pegaum( $sql );
	
	$sql = "SELECT kitid,kitdescricao,kitflagpedcli
			FROM pse.kitpedagoclinico
			WHERE 1 = 1 ".$where."	
			ORDER BY kitflagpedcli desc";	
	$dados = $db->carregar( $sql );
	echo "	<table style='border-bottom: 0px;' width='100%' bgcolor='#f5f5f5' cellSpacing='1' cellPadding='3' align='center'>
			<tr>
				<td class='SubTituloEsquerda' colspan='3'>4.2 Informe os materiais já recebidos pela SECRETARIA e quantidade, por item e kit completo.</td>
			</tr>
			<tr>
				<td align='center' style='background: rgb(238, 238, 238)'></td>
				<td align='center' style='background: rgb(238, 238, 238)' width='10%'><b>ESCOLAS</b></td>
				<td align='center' style='background: rgb(238, 238, 238)' width='10%'><b>ESF</b></td>
			</tr>
			<tr>
				<td colspan='3' style='background: rgb(238, 238, 238)'><b>MATERIAIS PEDAGÓGICOS</b></td>
			</tr>		
		";
		
	foreach($dados as $campos){
		if($conta1==0){
			echo "	
			<!--<tr>
				<td class='SubTituloEsquerda' style='background: rgb(238, 238, 238)'>QUANTITATIVO KITS COMPLETOS MATERIAIS PEDAGÓGICOS</td>
				<td align='center' style='background: rgb(238, 238, 238)'><input class='normal' maxlength='6' type='hidden' value='".$totalPESC."' name='totalPesc' id='totalPesc' size='10' style='text-align:center'><div id='totalpedagogicoEscola'><b>".$totalPESC."</b></div></td>
				<td align='center' style='background: rgb(238, 238, 238)'><input class='normal' maxlength='6' type='hidden' value='".$totalPESF."' name='totalPesf' id='totalPesf' size='10' style='text-align:center'><div id='totalpedagogicoESF'><b>".$totalPESF."</b></div></td>
						
			</tr>-->
			<tr>
				<td colspan='3' style='background: rgb(238, 238, 238)'><b>EQUIPAMENTOS CLÍNICOS</b></td>
			</tr>
			";
		}
		$sql = "SELECT skcquantescola, skcquantesf
				FROM pse.secrekitcompleto
				WHERE empid=$empid and kitid = $campos[kitid]";	
		$valores = $db->pegaLinha( $sql );
		$valoresc = ($valores['skcquantescola']==''?'':$valores['skcquantescola']);
		$valoresf = ($valores['skcquantesf']==''?'':$valores['skcquantesf']);
		echo "
			<tr>
				<td>".$campos['kitdescricao']."<input type='hidden' name='kitid[]' id='kitid[]' value='$campos[kitid]'></td>";
				if($campos['kitflagpedcli']=='p'){
				echo "<td align='center'><input class='normal' maxlength='6' type='text' value='$valoresc' name='kitidescola_$campos[kitid]' size='10' id='kitidescola_$campos[kitid]' style='text-align:center' onkeypress='return somenteNumeros(event);' onKeyUp='calculaTotalPedagogico(\"$campos[kitflagpedcli]\",$campos[kitid],this.value)'></td>
					  <td align='center'><input class='normal' maxlength='6' type='text' value='$valoresf' name='kitidesf_$campos[kitid]' size='10' id='kitidesf_$campos[kitid]' style='text-align:center' onkeypress='return somenteNumeros(event);' onKeyUp='calculaTotalPedagogico(\"$campos[kitflagpedcli]\",$campos[kitid])'></td>";
				}
				else {
				echo "<td align='center'><input class='normal' maxlength='6' type='text' value='$valoresc' name='kitidescola_$campos[kitid]' size='10' id='kitidescola_$campos[kitid]' style='text-align:center' onkeypress='return somenteNumeros(event);' onKeyUp='calculaTotalClinico(\"$campos[kitflagpedcli]\",$campos[kitid])'></td>
					  <td align='center'><input class='normal' maxlength='6' type='text' value='$valoresf' name='kitidesf_$campos[kitid]' size='10' id='kitidesf_$campos[kitid]' style='text-align:center' onkeypress='return somenteNumeros(event);' onKeyUp='calculaTotalClinico(\"$campos[kitflagpedcli]\",$campos[kitid])'></td>";	
				}			
				
		echo "</tr>";
		$conta1 = ($conta1-1);
		
	}
	echo "	
			<!--<tr>
				<td class='SubTituloEsquerda' style='background: rgb(238, 238, 238)'>QUANTITATIVO KITS COMPLETOS EQUIPAMENTOS CLÍNICOS</td>
				<td align='center' style='background: rgb(238, 238, 238)'><input class='normal' maxlength='6' type='hidden' value='".$totalCESC."' name='totalCesc' id='totalCesc' size='10' style='text-align:center'><div id='totalclinicoescola'><b>".$totalCESC."</b></div></td>
				<td align='center' style='background: rgb(238, 238, 238)'><input class='normal' maxlength='6' type='hidden' value='".$totalCESF."' name='totalCesf' id='totalCesf' size='10' style='text-align:center'><div id='totalclinicoesf'><b>".$totalCESF."</b></div></td>
			</tr>-->
		</table>		
	";
	
}

function perguntasULI($pergunta,$espid,$anoref=null,$objeto=null){
	global $db;
	
	if( $espid == '' || empty($espid)){
			print "<script>"
				. "    alert('Você não pode acessar essa área até fazer o cadastro em Identificação!');"
				. "    history.back(-1);"
				. "</script>";
			
			die;
	}	
	
	$usucpf = $_SESSION['usucpf'];
	$pflcod = pegaPerfil($usucpf);
	
	$sql = "select p.pulid, p.pulboleana, p.pulflagitem, p.pulflagmultiescolha, p.puldescricao
			from pse.pulpergunta p
			where p.pulid = $pergunta";
	$dados_pergunta = $db->pegaLinha($sql);
	
	//***** Verificação se a pergunta está cadastrada no BD	
	if($dados_pergunta=='') {
		echo "<script>alert('Não existe pergunta com os parâmetros informados!');</script>";
	}
	else {
		
		echo "<tr id='itens".$dados_pergunta['pulid']."'><td colspan='2'>
				<table width='100%' bgcolor='#f5f5f5'>";
		//***** Insere a pergunta
		echo "<tr>
				<td class='SubTituloEsquerda' colspan='2'>".
					$dados_pergunta['puldescricao'].
				"</td>
			  </tr>";
			  
		//***** Verifica se a pergunta é boleana	  
		if($dados_pergunta['pulboleana']=='s'){
			//Verifica se existe resposta já grava no BD	
			$sql = "SELECT rulflagresposta from pse.rulresposta
					WHERE pulid = ".$dados_pergunta['pulid']." and espid = $espid";
			$radios = $db->pegaUm($sql);	
			$checkNao = "";
			$checkSim = "";
			$radios = ($radios==''?'':$radios);
			if($radios!="")
				$radios != "s" ? $checkNao = "checked" : $checkSim = "checked";
			
			echo "<tr>
					<td style='text-align: left' colspan='2'>
						<input type='radio' name='rulflagresposta_".$dados_pergunta['pulid']."' id='rulflagresposta_".$dados_pergunta['pulid']."' $checkSim value='s' onclick='check(\"s\")'> Sim
						<input type='radio' name='rulflagresposta_".$dados_pergunta['pulid']."' id='rulflagresposta_".$dados_pergunta['pulid']."' $checkNao value='n' onclick='check(\"n\")'> Não		
				 	</td>
				 </tr>";	
		}
		
		//***** verifica se a pergunta tem itens
		if($dados_pergunta['pulflagitem']=='s'){
			$sql = "select iulid,pulid,iul_iulid,iuldescricao,iulflagtexto,iulflagvalor,iulflagsubitem
					from pse.iulitempergunta
					where pulid = $pergunta";
			$dados_itempergunta = $db->carregar($sql);

			//***** verifica o objeto informado (na chamada da função se for para usar "Agrupador", tem que colocar o parametro, caso contrário será "CheckBox ou Radio")
			if(empty($objeto)){

				foreach($dados_itempergunta as $item){
					
					//***** Busca os itens selecionados para marcar
					$sql = "select count(*) 
							from pse.isuitemselecionado it
							where it.rulid = (select rulid from pse.rulresposta r where espid=$espid and pulid=$pergunta) 
								and iulid = $item[iulid]";
					$check = $db->pegaUm($sql);
					$check = ($check==0?'':'checked');
					
					//echo $item['iuldescricao']." - ".$check."<br>";
					
					//***** Verifica se cada item tem um subitem 
					if($item['iulflagsubitem']=='n'){
						echo "<tr>
								<td style='text-align: left' colspan='2'>";
								
									//***** verifica se o item é de multipla escolha, se for coloca checkbox, senão Radio 								
									if($dados_pergunta['pulflagmultiescolha']=='s')
										echo "<input type='checkbox' name='perg_".$pergunta."[]' id='perg_".$pergunta."[]' $check value='".$item['iulid']."'> ".$item['iuldescricao'];
									else
										echo "<input type='radio' name='perg_".$pergunta."[]' id='perg_".$pergunta."[]' $check value='".$item['iulid']."'> ".$item['iuldescricao'];
									
									//***** Verifica se o item tem um texto (Ex: outras)	
									if($item['iulflagtexto']=='s'){
										$sql = "select tultexto
												from pse.tultextoitemselecionado it
												inner join
													pse.isuitemselecionado i on i.isuid=it.isuid			
												where i.iulid = $item[iulid]";
										$texto = $db->pegaUm($sql);
			
										echo " ".campo_texto('tultexto_'.$item['iulid'],'S','S','',50,100,'','','','','','id=tultexto_'.$item['iulid'],'',$texto);
									}		
						echo "  </td>
						 	  </tr>";
					}
					
					//***** Tendo subitem *************
					else {
						echo "<tr id='itens".$dados_pergunta['pulid']."'>
									<td colspan='2'>
										<table width='100%' bgcolor='#f5f5f5'>";
											combosSubitem($pergunta,$item['iulid'],$espid);
						echo "			</table>	
									</td>
								</tr>";
					}						 
						
				}
				
			}
			else {
				$sql = "select iulid as codigo,iuldescricao as descricao
						from pse.iulitempergunta
						where pulid = $pergunta";
				$dadosPergunta = $db->carregar($sql);
				$sql = "select  t.iulid as codigo,t.iuldescricao as descricao
						from pse.isuitemselecionado i
						inner join
						pse.iulitempergunta t on t.iulid=i.iulid
						inner join
						pse.pulpergunta p on p.pulid=t.pulid
						inner join
						pse.rulresposta r on r.rulid=i.rulid
						where p.pulid = $pergunta and r.espid=$espid";
				$dadosGravados = $db->Carregar($sql);
				
				echo "<tr>
    					<td colspan='2'  style='background: rgb(238, 238, 238)'>";
							$Atores = new Agrupador( 'formulario' );
							$Atores->setOrigem( 'origem', null, $dadosPergunta );
							$Atores->setDestino( 'destino', null, $dadosGravados);
							$Atores->exibir();
				echo "	</td>
					</tr>";
			}
			
		}
	
	echo "</table></td></tr>";	
	}
		
}

function gravaPerguntas($post,$espid,$ano){
	global $db;
		$idpergunta = explode(",",$_REQUEST['rulid']);
		if( $espid == '' || empty($espid)){
			print "<script>"
				. "    alert('Você não pode acessar essa área até fazer o cadastro em Identificação!');"
				. "    history.back(-1);"
				. "</script>";
			
			die;
		}	
		if( is_array($idpergunta) ){
			foreach($idpergunta as $pergunta){
				$s = '';
				$sql = "SELECT rulid FROM pse.rulresposta
						WHERE pulid = $pergunta and espid = $espid";
				$rulid = $db->pegaUm($sql);
	
				$sql = "SELECT pulboleana FROM pse.pulpergunta
						WHERE pulid = $pergunta";
				$bol = $db->pegaUm($sql);
				if($bol=='s'){
					$s = $_REQUEST['rulflagresposta_'.$pergunta];
				}
				else
					$s = 's';
	
				$s = ($s==''?'s':$s);
					if($rulid==''){	
						$sql = "INSERT INTO pse.rulresposta
								(pulid,espid,paqid,rulflagresposta)
								VALUES
								($pergunta,$espid,$ano,'$s')
								RETURNING rulid";
						$rulid = $db->pegaUm($sql);
					}
					else {
						
						$sql = "UPDATE pse.rulresposta SET
								rulflagresposta = '$s'
								WHERE rulid = $rulid";
						$db->executar($sql);
						$db->commit();
						$sql = "DELETE FROM pse.tultextoitemselecionado
								WHERE isuid in (select isuid from pse.isuitemselecionado where rulid= ".$rulid.")";
						$db->executar($sql);
						$db->commit();
						$sql = "DELETE FROM pse.isuitemselecionado WHERE rulid = ".$rulid;
						$db->executar($sql);
						
						$db->commit();
					}
				
	
				$valores = $_REQUEST['perg_'.$pergunta];
				
				if(!empty($valores)){
					foreach ($valores as $item) {
						$sql = "INSERT INTO pse.isuitemselecionado
								(iulid,rulid)
								VALUES
								($item,$rulid)
								RETURNING isuid";
						$isuid = $db->pegaUm($sql);
						
						$texto = $_REQUEST['tultexto_'.$item];
						if(!empty($texto)){
							$sql = "INSERT INTO pse.tultextoitemselecionado
									(isuid,tultexto)
									VALUES
									($isuid,'$texto')";
							$db->executar($sql);
						}
					}
				}
				
				if(!empty($_REQUEST['destino'])){
					foreach( $_REQUEST['destino'] as $item ){
						$sql = "INSERT INTO pse.isuitemselecionado
								(iulid,rulid)
								VALUES
								($item,$rulid)";
						$db->executar($sql);		
					}
				}
				
				$db->commit();
			}
			echo "<script>alert('Operação realizada com sucesso!');</script>";	
		} else {
			echo "<script>alert('Erro!');</script>";	
		}
		
}

function combosSubitem($pergunta,$item,$espid){
	global $db;
	
	$usucpf = $_SESSION['usucpf'];
	$pflcod = pegaPerfil($usucpf);
	
	$sub = $item;
	$pergOrigem = $pergunta;
			
	switch ($pergunta){
		case 18:
			
			$arritem = array();
			$sql = "SELECT iulid FROM pse.iulitempergunta
					WHERE pulid = $pergunta and iuldescricao<>'Outros' order by iulid";
			$dados = $db->carregar($sql);
			$z = 0;
			foreach($dados as $dados){
				$arritem[$z] = $dados['iulid'];
				$z++;
			}
			//$arritem		= array(121,122,123,124,125,126);
			//$arritem		= array(362,363,364,365,366,367);
			$key 			= array_search($item, $arritem);
			$pergunta		= 12;
			$arrnewitem = array();
			$sql = "SELECT iulid FROM pse.iulitempergunta
					WHERE pulid = $pergunta and iuldescricao<>'Outros' order by iulid";
			$dados = $db->carregar($sql);
			$z = 0;
			foreach($dados as $dados){
				$arrnewitem[$z] = $dados['iulid'];
				$z++;
			}
			//$arrnewitem		= array(76,77,78,79,80,81);
			//$arrnewitem		= array(317,318,319,320,321,322);
			$item			= $arrnewitem[$key];
		break;
	}

	if($pergOrigem==18){
	$sql = "SELECT p.iuldescricao
			FROM pse.iulitempergunta p
			INNER JOIN
				pse.isuitemselecionado its ON its.iulid=p.iulid
			INNER JOIN
				pse.rulresposta r ON r.rulid=its.rulid
			WHERE p.pulid=$pergunta AND r.espid=$espid AND p.iulid = $item";
	}
	else {
	$sql = "SELECT p.iuldescricao
			FROM pse.iulitempergunta p
			INNER JOIN
				pse.iulitempergunta its ON its.iulid=p.iulid
						WHERE p.pulid=$pergunta AND p.iulid = $item";
	}		
	$subItens = $db->pegaLinha($sql);
	
	if($subItens){
		echo "	<tr>
					<td style='text-align: left' colspan='2'><b>".$subItens['iuldescricao']."<td>
				</tr>
		 	  	<tr>
					<td align='right' class='SubTituloDireita'>Subação:</td>		
		 	  		<td style='background: rgb(238, 238, 238)'>";
					//***** Tabela Subacao
						$sql = "select subid as codigo,subdescricao as descricao
								from pse.subacao
								where iulid = $sub";
								$dados_subacao = $db->carregar($sql);
								$db->monta_combo( "subacao_".$sub, $sql, 'S', 'Selecione...', '', '', '', '450', 'S','subacao_'.$sub);
		echo "		</td>
		 	  	</tr>
		 	  	<tr>
					<td align='right' class='SubTituloDireita'>Periodicidade:</td>		
		 	  		<td style='background: rgb(238, 238, 238)'>";
						//***** Tabela Periodicidade 
						$sql = "select pedid as codigo,peddescricao as descricao
								from pse.periodicidade";
						$dados_periodicidade = $db->carregar($sql);
						$db->monta_combo( "periodicidade_".$sub, $sql, 'S', 'Selecione...', '', '', '', '450', 'S','periodicidade_'.$sub);
		echo "		</td>
		 	  	</tr>
		 	  	<tr>
					<td align='right' class='SubTituloDireita'>Parcerias:</td>		
		 	  		<td style='background: rgb(238, 238, 238)'>";
						//***** Tabela Parceria
						$sql = "select parid as codigo,pardescricao as descricao
								from pse.parceria";
						$dados_parceria = $db->carregar($sql);
						$db->monta_combo( "parceria_".$sub, $sql, 'S', 'Selecione...', '', '', '', '450', 'S','parceria_'.$sub);
		echo "		</td>
		 	  	</tr>
		 	  	<tr>
					<td align='right' class='SubTituloDireita'>Participantes:</td>		
		 	  		<td style='background: rgb(238, 238, 238)'>";
						//***** Tabela Participante
						$sql = "select patid as codigo,patdescricao as descricao
								from pse.participante";
						$dados_participante = $db->carregar($sql);
						$db->monta_combo( "participante_".$sub, $sql, 'S', 'Selecione...', '', '', '', '450', 'S','participante_'.$sub);
		echo "		</td>
		 	  	</tr>
		 	  	<tr>
					<td align='right' class='SubTituloDireita'>Material Didático Pedagógico Utilizado:</td>		
		 	  		<td style='background: rgb(238, 238, 238)'>";
						//***** Tabela Material Didático Pedago";
						$sql = "select mdpid as codigo,mdpdescricao as descricao
								from pse.materialdidatpedago";
						$dados_materialdidatpedago = $db->carregar($sql);
						$db->monta_combo( "material_".$sub, $sql, 'S', 'Selecione...', '', '', '', '450', 'S','material_'.$sub);
		echo "		</td>
		 	  	</tr>";
		if($pflcod == ESCOLA_ESTADUAL || $pflcod == ESCOLA_MUNICIPAL || $pflcod == SUPER_USUARIO){
			echo "	<tr bgcolor='#cccccc'>
						<td style='text-align: center' colspan='2'>
							<input type='button' class='botao' name='btsalvar' value='incluir' onclick='submeteCombos(".$sub.")'>
						</td>
					</tr>";
		}
		
		echo "		</form><tr>
						<td colspan='2' style='background: rgb(238, 238, 238)'>
							<div id='lista_".$sub."'><script>CarregaLista($sub);</script></div>
						</td>
					</tr>";			
	}	
}

function GravarSubItens($post){
	global $db;
	
	//Resposta	
	$sql = "SELECT rulid FROM pse.rulresposta
			WHERE pulid=$_REQUEST[pergunta] AND espid=$_REQUEST[espid] AND paqid = $_REQUEST[ano]";
	$rulid = $db->pegaUm($sql);
	if(!$rulid){
		$sql = "INSERT INTO pse.rulresposta
				(pulid,espid,paqid,rulflagresposta)
				VALUES	
				($_REQUEST[pergunta],$_REQUEST[espid],$_REQUEST[ano],'s')
				RETURNING rulid";
		$rulid = $db->pegaUm($sql);
	} 
	
	//Inclusão do item
	$sql = "SELECT isuid FROM pse.isuitemselecionado
			WHERE rulid=$rulid AND iulid = $_REQUEST[item]";
	$isuid = $db->pegaUm($sql);
	if(!$isuid){
		$sql = "INSERT INTO pse.isuitemselecionado
				(iulid,rulid)
				VALUES	
				($_REQUEST[item],$rulid)
				RETURNING isuid";
		$isuid = $db->pegaUm($sql);
	}
	
	//Conjunto de subitens
	$sql = "INSERT INTO pse.subitemselecionado
			(subid,parid,patid,mdpid,pedid)
			VALUES	
			($_REQUEST[subacao],$_REQUEST[parceria],$_REQUEST[participante], 
			 $_REQUEST[material],$_REQUEST[periodicidade])
			RETURNING sisid";
	$subid = $db->pegaUm($sql);
	
	//Relação do item com os subitens
	$sql = "INSERT INTO pse.asuselecionada
			(sisid,isuid)
			VALUES	
			($subid,$isuid)";
	$db->executar($sql);
	
	$db->commit();
	
}

function carregaListaSubitens($pergunta,$espid,$sub){
	global $db;
	$usucpf = $_SESSION['usucpf'];
	$pflcod = pegaPerfil($usucpf);
		
	$sql = "SELECT	";
	if($pflcod == ESCOLA_ESTADUAL || $pflcod == ESCOLA_MUNICIPAL || $pflcod == SUPER_USUARIO)
		$sql.=	"'<a href=\"#\" onclick=\"ExcluirSubItem('||asu.asuid||',$sub);\" title=\"Excluir registro\"><img src=\"../imagens/excluir.gif\" style=\"cursor:pointer;\" border=\"0\"></a>'";
	else {
		$sql.= "''";
	}
	$sql.= "as acao, sa.subdescricao, pe.peddescricao, pa.pardescricao, par.patdescricao, ma.mdpdescricao
			FROM pse.subitemselecionado si
			INNER JOIN
				pse.subacao sa ON sa.subid=si.subid
			INNER JOIN
				pse.periodicidade pe ON pe.pedid=si.pedid
			INNER JOIN
				pse.parceria pa ON pa.parid=si.parid
			INNER JOIN
				pse.participante par ON par.patid=si.patid
			INNER JOIN
				pse.materialdidatpedago ma ON ma.mdpid=si.mdpid
			INNER JOIN
				pse.asuselecionada asu ON asu.sisid=si.sisid
			INNER JOIN
				pse.isuitemselecionado its ON its.isuid=asu.isuid
			INNER JOIN
				pse.rulresposta r ON r.rulid=its.rulid
			WHERE r.pulid=$pergunta AND r.espid= $espid AND its.iulid = $sub"; 
			
	$cabecalho	= array( "Ação", "Subação", "Periodicidade", "Parceria", "Participante", "Materiais");
	$alinha		= array("center","left","left","left","left","left");
	$tamanho	= array("5%","19%","19%","19%","19%","19%");
	$db->monta_lista( $sql, $cabecalho, 25, 10, 'N', 'center', '', '',$tamanho,$alinha);
	
}

function excluiSubItem($asuid){
	global $db;
	$sql = "SELECT isuid FROM pse.asuselecionada
			WHERE asuid=$asuid";
	$isuid = $db->pegaUm($sql);
	//Ocorreu um erro no dia 06/01/2010 no arquivo compInecessidadeCarencia03 onde o isuid veio em branco. Aqui eu vejo se ele existe, se não ele não executa.
	if($isuid <> ''){
		//dbg($isuid);
		$sql = "DELETE FROM pse.asuselecionada
				WHERE asuid=$asuid";
		$db->executar($sql);
		//dbg($sql);
		$sql = "SELECT count(*) FROM pse.asuselecionada
				WHERE isuid=$isuid";
		$res = $db->pegaUm($sql);
		//dbg($res);
		if($res==0){
			$sql = "DELETE FROM pse.isuitemselecionado
					WHERE isuid=$isuid";
			$db->executar($sql);
		}	
	}
	//dbg($asuid,1);		
	$db->commit();
}
/*
function preparaPesquisa($tipo){
	global $db;
	if($tipo=='s'){
		
	}
	else if($tipo=='e'){
		
	}
	else if($tipo=='u'){
		
		$sql = "SELECT pulid as codigo, puldescricao as descricao FROM pse.pulpergunta";
		$sql = $db->carregar($sql);
		echo $db->monta_combo( "perguntas", $sql, 'S', 'Selecione...', 'filtro', '', '', '', 'N','id="perguntas"');
		
		echo "<div id='filtros'></div>";

	}
}

function mostraFiltro($pulid){
	global $db;
	$sql = "select p.pulid, p.pulboleana, p.pulflagitem, p.pulflagmultiescolha, p.puldescricao
			from pse.pulpergunta p
			where p.pulid = $pulid";
	$sql = $db->pegaLinha($sql);

	if($sql['pulboleana']=='s')
		echo "É boleana!!!";
	else
		echo "não é boleana!!!";

	echo "<br>";	
		
	if($sql['pulflagitem']=='s'){
		$sql = "select iulid as codigo, iuldescricao as descricao --,pulid,iul_iulid,iulflagtexto,iulflagvalor,iulflagsubitem
				from pse.iulitempergunta
				where pulid = $pulid";
		$sql = $db->carregar($sql);
		echo $db->monta_combo( "itens", $sql, 'S', 'Selecione...', '', '', '', '', 'N','id="itens"');

	}
		
	
}
*/

function pegaMuncodEscola( $entid ){
	global $db;
	
	$sql = "SELECT
				m.muncod
			FROM
				territorios.municipio m
			INNER JOIN
				entidade.endereco e ON e.muncod = m.muncod
			WHERE
				e.entid = ".$entid;
	
	return $db->pegaUm( $sql );
}

function pegaPortaria( $muncod ){
	global $db;
	
	$sql = "SELECT
				porano + 1
			FROM
				pse.portariapse
			WHERE
				pormunicipio = '".$muncod."'";
	
	return $db->pegaUm( $sql );
}

function pegaNovoEmpid( $muncod ){
	global $db;
	
	$sql = "SELECT
				empid
			FROM
				pse.estadomunicipiopse em
			INNER JOIN
				pse.portariapse pp ON pp.pormunicipio = em.muncod
			WHERE
				muncod = '".$muncod."' AND
				(empano = porano + 1)";
	
	return $db->pegaUm( $sql );	
}

function pegaNovoEspid( $entid, $portaria ){
	global $db;
	
	$sql = "SELECT
				espid
			FROM
				pse.escolapse
			WHERE
				entid = ".$entid." AND
				espano = ".$portaria;
	
	return $db->pegaUm( $sql );	
}

/**
 * Recupera o(s) perfil(is) do usuário no módulo
 * 
 * @return array $pflcod
 */
function arrayPerfil() {
    global $db;

    /*     * * Executa a query para recuperar os perfis no módulo ** */
    $sql    = "SELECT
				pu.pflcod
			FROM
				seguranca.perfilusuario pu
			INNER JOIN 
				seguranca.perfil p ON p.pflcod = pu.pflcod
								  AND p.sisid = 65
			WHERE
				pu.usucpf = '" . $_SESSION['usucpf'] . "'
			ORDER BY
				p.pflnivel";
    $pflcod = $db->carregarColuna( $sql );

    /*     * * Retorna o array com o(s) perfil(is) ** */
    return (array) $pflcod;
}

function carregarMenuAbasPse() {
    
	//pega perfil do usuario
	$pfls = arrayPerfil();
	
 	if ( in_array(SUPER_USUARIO, $pfls) ) {

 		$menu = array( 0 => array( "id" => 1, "descricao" => "Cadastro Secretaria", 						"link" => "/pse/pse.php?modulo=principal/listaMunicipios&acao=A" ),
				       1 => array( "id" => 2, "descricao" => "Cadastro Escola", 							"link" => "/pse/pse.php?modulo=principal/ListarEscolas&acao=A" ),
				       2 => array( "id" => 3, "descricao" => "Unidade Local Integrada", 					"link" => "/pse/pse.php?modulo=principal/ULILista&acao=A" ),
				       3 => array( "id" => 4, "descricao" => "Cadastro Termo de Compromisso", 				"link" => "/pse/pse.php?modulo=principal/listaMunicipiosTermo&acao=A" ),
				       4 => array( "id" => 5, "descricao" => "Semana Saúde na Escola", 						"link" => "/pse/pse.php?modulo=principal/listaMunicipiosSemana&acao=A" ),
				       5 => array( "id" => 6, "descricao" => "Monitoramento de Ações", 						"link" => "/pse/pse.php?modulo=principal/ListarEscolasMonitoramento&acao=A" ),
				       6 => array( "id" => 7, "descricao" => "Brasil Carinhoso", 							"link" => "/pse/pse.php?modulo=principal/ListarEscolasCarinhoso&acao=A" ),
				       7 => array( "id" => 8, "descricao" => "Componente III", 								"link" => "/pse/pse.php?modulo=principal/listaMunicipiosComp3&acao=A" ),
				       8 => array( "id" => 9, "descricao" => "Justificativa para o não alcance de metas", 	"link" => "/pse/pse.php?modulo=principal/justificativaMetas&acao=A" ),
				       9 => array( "id" => 10, "descricao" => "Componente 2", 								"link" => "/pse/pse.php?modulo=principal/listaComp2&acao=A" ),
				       10 => array( "id" => 11, "descricao" => "Componente 3", 								"link" => "/pse/pse.php?modulo=principal/listaComp3&acao=A" ),
                                       11 => array( "id" => 11, "descricao" => "Nutrisus",       "link" => "/pse/pse.php?modulo=principal/listaNutrisus&acao=A" )

				    );
 	}
 	elseif ( in_array(EDUCADOR_ESCOLA, $pfls)) {

 		$menu = array( 0 => array( "id" => 1, "descricao" => "Componente 2", 	"link" => "/pse/pse.php?modulo=principal/listaComp2&acao=A" ),
                               1 => array( "id" => 2, "descricao" => "Nutrisus",       "link" => "/pse/pse.php?modulo=principal/listaNutrisus&acao=A" )
                    );
 	}
        elseif ( in_array(SECRETARIA_MUNICIPAL, $pfls)|| in_array(SECRETARIA_ESTADUAL, $pfls) ) {

 		$menu = array( 0 => array( "id" => 1, "descricao" => "Componente 2", 	"link" => "/pse/pse.php?modulo=principal/listaComp2&acao=A" ),
                               1 => array( "id" => 2, "descricao" => "Nutrisus",       "link" => "/pse/pse.php?modulo=principal/listaNutrisus&acao=A" ),
                    	       2 => array( "id" => 3, "descricao" => "Componente III", 	"link" => "/pse/pse.php?modulo=principal/listaComp3&acao=A" )
);
 	}
            elseif ( in_array(MEC, $pfls) ) {

 		$menu = array( 0 => array( "id" => 1, "descricao" => "Componente 2", 	"link" => "/pse/pse.php?modulo=principal/listaComp2&acao=A" ),
                               1 => array( "id" => 2, "descricao" => "Nutrisus",       "link" => "/pse/pse.php?modulo=principal/listaNutrisus&acao=A" ),
                               2 => array( "id" => 3, "descricao" => "Componente III", 	"link" => "/pse/pse.php?modulo=principal/listaComp3&acao=A" )

                    );
 	}
 	else{
 		
 		$menu = array( 0 => array( "id" => 1, "descricao" => "Componente 2", 								"link" => "/pse/pse.php?modulo=principal/listaComp2&acao=A" ),
			       1 => array( "id" => 2, "descricao" => "Componente 3", 								"link" => "/pse/pse.php?modulo=principal/listaComp3&acao=A" )
				    );
 	}
 	
    return $menu;
    
}
?>