
<?php

if( $_POST['arqid'] != '' ){
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec();
	$arquivo = $file->getDownloadArquivo($_POST['arqid']);
	echo"<script>window.location.href = window.location.href;</script>";
	exit;
}

//
function carregaOcupacao( $request ){
	global $db;
	$sql = "SELECT ocuid as codigo, ocudesc as descricao FROM projovemurbano.ocupacao WHERE ocustatus='A' ORDER BY ocudesc";
	$dados = $db->carregar($sql);
	?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<?php
	foreach($dados as $dado){
		?>
	<tr>
		<td><?=$dado['descricao'] ?></td>
	</tr>
	<?php
}
?>
</table>
<?php
}

function carregaProgramasBeneficiarios( $request ){
	global $db;
	$sql = "SELECT pbeid as codigo, pbedesc as descricao FROM projovemurbano.programabeneficiario WHERE pbestatus='A' ORDER BY pbedesc";
	$dados = $db->carregar($sql);
	?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<?php
	foreach($dados as $dado){
		?>
	<tr>
		<td><?=$dado['descricao'] ?></td>
	</tr>
	<?php
}
?>
</table>
<?php
}

function carregaDeficiencia( $request ){
	global $db;
	$sql = "SELECT tdeid as codigo, tdedesc as descricao FROM projovemurbano.tipodeficiencia WHERE tdestatus='A' ORDER BY tdedesc";
	$dados = $db->carregar($sql);
	?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<?php
	foreach($dados as $dado){
		?>
	<tr>
		<td><?=$dado['descricao'] ?></td>
	</tr>
	<?php
}
?>
</table>
<?php
}

function carregaRaca( $request ){
	global $db;
	$sql = "SELECT craid as codigo, cradesc as descricao FROM projovemurbano.corraca WHERE crastatus='A' ORDER BY cradesc";
	$dados = $db->carregar($sql);
	?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<?php
	foreach($dados as $dado){
		?>
	<tr>
		<td><?=$dado['descricao'] ?></td>
	</tr>
	<?php
}
?>
</table>
<?php
}

function carregaNacionalidade( $request ){
	$naturalidade = array(0=>array("codigo"=>"B","descricao"=>"Brasileira"),1=>array("codigo"=>"E","descricao"=>"Estrangeira"));
	$dados = $naturalidade;
	?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<?php
	foreach($dados as $dado){
		?>
	<tr>
		<td><?=$dado['descricao'] ?></td>
	</tr>
	<?php
}
?>
</table>
<?php
}

function carregaTranstorno( $request ){
	global $db;
	$sql = "SELECT tdiid as codigo, tdidesc as descricao FROM projovemurbano.transdesininfancia WHERE tdistatus='A' AND tdiid not in (5) ORDER BY tdidesc";
	$dados = $db->carregar($sql);
	?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<?php
	foreach($dados as $dado){
		?>
	<tr>
		<td><?=$dado['descricao'] ?></td>
	</tr>
	<?php
}
?>
</table>
<?php
}

function carregaEstadoCivil( $request ){
	global $db;
	$sql = "SELECT escid as codigo, escdesc as descricao FROM projovemurbano.estadocivil WHERE escstatus='A' ORDER BY escdesc";
	$dados = $db->carregar($sql);
	?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<?php
	foreach($dados as $dado){
		?>
	<tr>
		<td><?=$dado['descricao'] ?></td>
	</tr>
	<?php
}
?>
</table>
<?php
}

function carregaSegmentoSocial( $request ){
	global $db;
	$sql = "SELECT ssoid as codigo, ssodesc as descricao FROM projovemurbano.segmentosocial WHERE ssostatus='A' ORDER BY ssodesc";
	$dados = $db->carregar($sql);
	?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<?php
	foreach($dados as $dado){
		?>
	<tr>
		<td><?=$dado['descricao'] ?></td>
	</tr>
	<?php
}
?>
</table>
<?php
}

function carregaUF( $request ){
	global $db;
	$sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
	$dados = $db->carregar($sql);
	?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<?php
	foreach($dados as $dado){
		?>
	<tr>
		<td><?=$dado['descricao'] ?></td>
	</tr>
	<?php
}
?>
</table>
<?php
}
function carrega_Municipio( $request ){
	global $db;
	//$sql = "SELECT estuf, muncod, mundescricao as mundsc FROM territorios.municipio WHERE estuf = '" . $request['endestuf'] . "' ORDER BY mundescricao municipio";
	$sql = "SELECT estuf, muncod, mundescricao as mundsc FROM territorios.municipio WHERE estuf = 'MG' ORDER BY mundescricao LIMIT 20";
	$dados = $db->carregar($sql);
	?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<?php
	foreach($dados as $dado){
		?>
	<tr>
		<td><?=$dado['mundsc'] ?></td>
	</tr>
	<?php
}
?>
</table>
<?php
}

function carregaNucleoEscola( $request ){
	global $db;

	// Adaptação para o perfil Diretor do Núcleo
	if(!$db->testa_superuser()) {
		$perfis = pegaPerfilGeral();
		if(in_array(PFL_DIRETOR_NUCLEO, $perfis)) {
			$inner_nucleo = "inner join projovemurbano.usuarioresponsabilidade ur on ur.usucpf='".$_SESSION['usucpf']."' and ur.entid=nes.entid AND rpustatus='A'";
		}
	}

	if($request['possuipolo']=="t") {

		$sql = "SELECT DISTINCT
					nuc.nucid as codigo, 
					'NÚCLEO '||nuc.nucid||', SEDE: '||(SELECT entnome FROM entidade.entidade ent INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = ent.entid WHERE nes.nucid=nuc.nucid AND nes.nuetipo='S')||COALESCE(', ANEXO: '||(SELECT entnome FROM entidade.entidade ent INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = ent.entid WHERE nes.nucid=nuc.nucid AND nes.nuetipo='A'),'') as descricao 
				FROM projovemurbano.nucleo nuc
				INNER JOIN projovemurbano.nucleoescola nes ON nes.nucid = nuc.nucid
				INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
				INNER JOIN projovemurbano.associamucipiopolo amp ON amp.munid = mun.munid    
				INNER JOIN projovemurbano.polo pol ON pol.polid = amp.polid 
				INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = pol.pmuid 
				{$inner_nucleo}
				WHERE 
					nuc.nucstatus='A' AND mun.munstatus='A' AND plm.pmustatus='A' 
					/*Retirado a pedido do Wallace - 08/05/2012*/
				  	/*AND nuc.nucid NOT IN (SELECT
				  							nuc2.nucid
				  						  FROM
				  						  	projovemurbano.nucleo nuc2
				  						  WHERE
				  						  	nuc2.nucqtdestudantes <= ( SELECT count(caeid) FROM projovemurbano.cadastroestudante cae WHERE cae.nucid = nuc2.nucid  ))*/
				  	AND pol.polstatus='A' ".(($_SESSION['projovemurbano']['pjuid'])?" AND pjuid='".$_SESSION['projovemurbano']['pjuid']."'":"").(($polid)?" AND pol.polid='".$polid."'":"");
				$nucleos = $db->carregar($sql);

	} else {

		$sql = "SELECT DISTINCT
					nuc.nucid as codigo, 
					'NÚCLEO '||nuc.nucid||', 
					SEDE: '||(SELECT entnome FROM entidade.entidade ent INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = ent.entid WHERE nes.nucid=nuc.nucid AND nes.nuetipo='S')||
					COALESCE(', ANEXO:'||(SELECT entnome FROM entidade.entidade ent INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = ent.entid WHERE nes.nucid=nuc.nucid AND nes.nuetipo='A'),'') as descricao 
				FROM 
					projovemurbano.nucleo nuc 
				INNER JOIN projovemurbano.nucleoescola nes ON nes.nucid = nuc.nucid
			    INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid 
			    INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = mun.pmuid 
			    {$inner_nucleo}
			    WHERE 
			  		nuc.nucstatus='A' AND mun.munstatus='A' AND plm.pmustatus='A'
			  		/*Retirado a pedido do Wallace - 08/05/2012*/
			  		/*AND nuc.nucid NOT IN (SELECT
				  							nuc2.nucid
				  						  FROM
				  						  	projovemurbano.nucleo nuc2
				  						  WHERE
				  						  	nuc2.nucqtdestudantes <= ( SELECT count(caeid) FROM projovemurbano.cadastroestudante cae WHERE cae.nucid = nuc2.nucid  ))*/ 
			  		AND pjuid='".$_SESSION['projovemurbano']['pjuid']."'";
			    $nucleos = $db->carregar($sql);

	}
	//ver( $sql );
	//return $nucleos;
	$dados = $db->carregar($sql);
	?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<?php
	foreach($dados as $dado){
		?>
	<tr>
		<td><?=$dado['descricao'] ?></td>
	</tr>
	<?php
}
?>
</table>
<?php
}

function carregaNucleoTurma( $request ){
	global $db;
	if($request['nucid']){
		verificaTurmaNucleo( $request['nucid'] );
		if(!$db->testa_superuser()) {
			$perfis = pegaPerfilGeral();

			if(in_array(PFL_DIRETOR_NUCLEO, $perfis)) {
				if( $_SESSION['projovemurbano']['entid'] ){
					$escola_diretor = "t.entid = ".$_SESSION['projovemurbano']['entid']." AND ";
				}else{
					$escola_diretor = "1=0 AND";
				}
			}
		}

		// Query original que lista todas as turmas, inclusive as com nenhum aluno
		$sql = "SELECT DISTINCT
         turid as codigo,
         turdesc||
         CASE WHEN nes.nuetipo = 'S' THEN ' SEDE ' ELSE ' ANEXO ' END||
         ', Total de Alunos: '||(SELECT count(*) FROM projovemurbano.cadastroestudante c WHERE c.turid = t.turid AND caestatus = 'A') as descricao
         FROM
         projovemurbano.turma t
         INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = t.entid AND nes.nucid = ".$request['nucid']."
         WHERE
         $escola_diretor
        t.nucid = ".$request['nucid']." AND turstatus = 'A'
        ORDER BY
        2"; 

         $dados = $db->carregar($sql);
         //$dados['bloq'] = $dados['bloq'] ? $dados['bloq'] : 'S';
	}else{
		//$sql = "SELECT estuf, muncod, mundescricao as mundsc FROM territorios.municipio WHERE estuf = '" . $request['endestuf'] . "' ORDER BY mundescricao municipio";
		$sql = "SELECT DISTINCT
                                    turid as codigo,
                                    turdesc||
                                    CASE WHEN nes.nuetipo = 'S' THEN ' SEDE ' ELSE ' ANEXO ' END||
                                    ', Total de Alunos: '||(SELECT count(*) FROM projovemurbano.cadastroestudante c WHERE c.turid = t.turid AND caestatus = 'A') as descricao
                                    FROM
                                    projovemurbano.turma t
                                    INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = t.entid AND nes.nucid = 1417
                                    WHERE

                                   t.nucid = 1417 AND turstatus = 'A'
                                   ORDER BY
                                   2
                           ";
		$dados = $db->carregar($sql);
	}
	?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<?php
	foreach($dados as $dado){
		?>
	<tr>
		<td><?=$dado['descricao'] ?></td>
	</tr>
	<?php
}
?>
</table>
<?php
}

if( !empty($_REQUEST['testaDados']) ){

	$retorno = '';

	if( $_REQUEST['testaDados']($_REQUEST) ){
		$retorno = 1;
	}
	echo $retorno;
	die();
}

function verificaProjovemcampo($dados){
	global $db;
	$sqlve = "
				SELECT
					TRUE
				FROM
					projovemcampo.estudante
				WHERE
					estcpf = '{$dados['caecpf']}'
				AND eststatus = 'A'";
	$retorno = $db->pegaUm( $sqlve );
	// ver($retorno,d);
	if($retorno == 't'){
	echo 1;
}else{
		echo 0;
}
die;
}

function verificarFaltasAluno( $dados ){

	global $db;

	$turid = $dados['turid'];
	$nucid = (int) $dados['nucid'];

	$parametros = array(
	    				 'turid' => $turid,
	    				 'nucid' => $nucid );

	$periodoAtual = retornaPeriodoAtual( $turid);

	if( is_array( $periodoAtual ) ){
		foreach( $periodoAtual as $periodo ){
	
			$dadosDiario = retornaDadosDiario( $turid, $periodo['periodo'] );
	
			foreach( $dadosDiario as $diario ){
	
				$componentesCurriculares  = listarComponenteCurricular( $dadosDiario['diaid'] );
	
				$qtdColunas= count( $componentesCurriculares );
					
				for ( $i = 0; $i < $qtdColunas; $i++ ) {
	
					$parametrosPresenca = array( 'difid' => $componentesCurriculares[$i]['difid']
					, 'caeid' => $_REQUEST['caeid'] );
					 
					$dadosPresenca = listaPresencaPorAlunoDiarioFechado( $parametrosPresenca );
	
					$qtdPresenca     = (!empty( $dadosPresenca['frqqtdpresenca'] ) ? $dadosPresenca['frqqtdpresenca'] : '0' );
					$qtdAulaDada     = (!empty( $dadosPresenca['difqtdauladada'] ) ? $dadosPresenca['difqtdauladada'] : '0' );
	//				$somaQuantidadePresenca += $qtdPresenca;
	//				$somaQuantidadeAulaDada += $qtdAulaDada;
					$tot += ( $qtdAulaDada - $qtdPresenca );
				}
			}
		}
	}
//			$qtdHorasMaxima = (int)( $somaQuantidadeAulaDada );
//			$qtdHorasMaximaOld = (int)( count( $periodoAtual) * 80 );
//			
//	$qtdHorasFalta =  (int)( $qtdHorasMaxima - $somaQuantidadePresenca );
//	$qtdHorasFaltaOld =  (int)( $qtdHorasMaximaOld - $somaQuantidadePresenca );
	
//	if( $qtdHorasFalta > 360 ){
	if( $tot > 360 ){
		echo 'inativo';
	}else{
		echo 'ativo';
	}
	die;
}

function listaPresencaPorAlunoDiarioFechado( $param )
{
	global $db;
	
	$difid = !empty($param['difid']) ? $param['difid'] : 0;
	$caeid = !empty($param['caeid']) ? $param['caeid'] : 0;
	
	$sql = "SELECT distinct(frq.frqid ), frq.frqqtdpresenca,  dif.difqtdauladada 
			FROM projovemurbano.frequenciaestudante frq 
			INNER JOIN projovemurbano.diariofrequencia dif 
				ON frq.difid = dif.difid 
			INNER JOIN projovemurbano.gradecurricular grd 
				ON dif.grdid = grd.grdid 
			INNER JOIN projovemurbano.componentecurricular coc 
				ON grd.cocid = coc.cocid 
			INNER JOIN projovemurbano.diario as dia ON dif.diaid = dia.diaid 
			INNER JOIN workflow.documento as doc 
				        ON doc.docid = dia.docid 
			WHERE frq.caeid = {$caeid} 
			AND frq.difid = {$difid} 
			AND doc.esdid = ".WF_ESTADO_DIARIO_FECHADO ." 
			GROUP BY frq.frqid, frq.frqqtdpresenca,  dif.difqtdauladada";

	$retorno = $db->pegaLinha( $sql );
	
	return $retorno;
}

function retornaPeriodoAtual( $turid ){

	global $db;
	if($turid){
		$sql = "
			SELECT distinct dia.perid as periodo
			        	FROM projovemurbano.diario dia
			        	INNER JOIN projovemurbano.periodocurso as per ON dia.perid = per.perid
			        	INNER JOIN projovemurbano.diariofrequencia as diaf on diaf.diaid = dia.diaid
			        	INNER JOIN projovemurbano.frequenciaestudante as freq on freq.difid = diaf.difid
			        	INNER JOIN workflow.documento as doc
					        ON doc.docid = dia.docid
			and dia.turid = $turid and doc.esdid = ".WF_ESTADO_DIARIO_FECHADO." order by periodo";
	
		$periodo = $db->carregar( $sql );
	}
	return $periodo;
}
if( $_REQUEST['reqTittle'] ){
	$_REQUEST['reqTittle']($_REQUEST);
	die();
}
/*Funções da aba Acompanhamneto de Frequência e Notas*/

function buscarComboPolo(){

	global $db;
	
	$perfis = pegaPerfilGeral();
	if(in_array(PFL_DIRETOR_POLO, $perfis)){
		$innerusuario = "INNER JOIN projovemurbano.usuarioresponsabilidade ur ON ur.usucpf= '".$_SESSION['usucpf']."' AND ur.polid=pol.polid AND rpustatus='A'";
	}
	
	if(in_array(PFL_DIRETOR_NUCLEO, $perfis)){
		$innerusuario = "LEFT JOIN projovemurbano.associamucipiopolo amp ON amp.polid = pol.polid
						LEFT JOIN projovemurbano.municipio mun ON amp.munid = mun.munid
						LEFT JOIN projovemurbano.nucleo nuc ON mun.munid = nuc.munid
						LEFT JOIN projovemurbano.nucleoescola nes ON nes.nucid = nuc.nucid
						INNER JOIN projovemurbano.usuarioresponsabilidade ur ON ur.usucpf= '".$_SESSION['usucpf']."'  AND ur.entid=nes.entid AND rpustatus='A' AND nuc.nucstatus = 'A'";
	}
	if($_SESSION['projovemurbano']['ppuid'] == 1){
		$sql = "SELECT
					pol.polid as codigo, 'POLO '||pol.polid as descricao
				FROM projovemurbano.polo pol
				INNER  JOIN projovemurbano.polomunicipio pmu ON pmu.pmuid = pol.pmuid
				$innerusuario
				WHERE
					pmustatus='A'
				AND polstatus='A'
				AND pjuid='".$_SESSION['projovemurbano']['pjuid']."'
				AND pmu.tprid is null
				ORDER BY pol.polid";
// 		ver($sql,d);
	}else{
		$sql = "SELECT
					pol.polid as codigo, 'POLO '||pol.polid as descricao
				FROM projovemurbano.polo pol
				INNER  JOIN projovemurbano.polomunicipio pmu ON pmu.pmuid = pol.pmuid
				$innerusuario
				WHERE
					pmustatus='A'
				AND polstatus='A'
				AND pjuid='".$_SESSION['projovemurbano']['pjuid']."'
				AND pmu.tprid is not null
				ORDER BY pol.polid";
	}
		$db->monta_combo('polid', $sql, 'S', 'Selecione', 'filtraNucleoCombo', '', '', '', 'N', 'polid');
		}

function buscarComboNucleo(){

	global $db;
	
	$perfis = pegaPerfilGeral();
	extract($_REQUEST);
	
	if(in_array(PFL_DIRETOR_NUCLEO, $perfis) ){
		$innerusuario = "INNER JOIN projovemurbano.usuarioresponsabilidade ur ON ur.usucpf= '".$_SESSION['usucpf']."' and ur.entid=nes.entid AND rpustatus='A'";
		
	}else{
		$polo = "AND pol.polid IN ($polid)";
	}
 
	if( $polid != '') {

		$sql = "SELECT DISTINCT
					nuc.nucid as codigo,
					'NUCLEO '||nuc.nucid as descricao
				FROM projovemurbano.nucleo nuc
                INNER JOIN projovemurbano.nucleoescola nes ON nes.nucid = nuc.nucid
                INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid
                INNER JOIN projovemurbano.associamucipiopolo amp ON amp.munid = mun.munid
                INNER JOIN projovemurbano.polo pol ON pol.polid = amp.polid
                INNER JOIN projovemurbano.polomunicipio plm ON plm.pmuid = pol.pmuid
                $innerusuario
	            WHERE
	            	nuc.nucstatus='A' AND mun.munstatus='A' AND plm.pmustatus='A'
	            AND pol.polstatus='A' $polo  AND pjuid='".$_SESSION['projovemurbano']['pjuid']."'
				";
// ver($sql,d);

	} else {

		$sql = "SELECT DISTINCT
					nuc.nucid as codigo,
					'NUCLEO '||nuc.nucid as descricao
				FROM
					projovemurbano.nucleo nuc
				INNER JOIN projovemurbano.nucleoescola nes ON nes.nucid = nuc.nucid
				INNER JOIN projovemurbano.municipio mun ON mun.munid = nuc.munid
				INNER JOIN projovemurbano.polomunicipio pmu ON pmu.pmuid = mun.pmuid
				LEFT  JOIN entidade.entidade             ent ON ent.entid  = nuc.entid
				$innerusuario
				WHERE
					nuc.nucstatus='A'
				AND mun.munstatus='A'
				AND pmu.pmustatus='A'
				AND nuc.ppuid = {$_SESSION['projovemurbano']['ppuid']}
				AND pjuid='".$_SESSION['projovemurbano']['pjuid']."'
			";

	}
	$db->monta_combo('nucid', $sql, 'S', 'Selecione o Núcleo', 'buscarTurmas', '', '', '', 'S', 'nucid');
}

function buscarComboTurma(){

	global $db;

	extract($_REQUEST);

	if( $nucid[0] != '' ){
		$sql = "SELECT DISTINCT
					turid AS codigo,
					turdesc || CASE WHEN nes.nuetipo = 'S'
					THEN ' SEDE '
					ELSE ' ANEXO ' END AS descricao
				FROM projovemurbano.turma t
				INNER JOIN projovemurbano.nucleoescola nes ON nes.entid = t.entid AND nes.nucid IN ($nucid)
				WHERE
					t.nucid IN ( $nucid )
				AND turstatus = 'A'
				ORDER BY 2";
	}else{
	$sql = "SELECT
	'' as codigo, '' as descricao";
	}
	//     ver($sql);
	$db->monta_combo('turid', $sql, S, 'Selecione', '', '', '', '', 'S', 'turid');
}
	 
function salvar_dados_conselho(){

	global $db;
// 	    	extract($arqid);
	//     	ver($_REQUEST,d);
	$sql = "SELECT 
				arqnome ||'.'|| arqextensao
			FROM
					projovemurbano.arquivo_turma a
			LEFT JOIN      public.arquivo    ag ON ag.arqid = a.arqid
			WHERE
				 turid = {$_REQUEST['turid']} AND arqstatus='A' ";
					 
 	$temarquivo = $db->pegaUm($sql);
 		
		if( $_FILES["arquivo"]['name']!=''&& $temarquivo && ($temarquivo != $_FILES["arquivo"]['name'] && !$_POST['caeid']) ){
			$sql = "UPDATE arquivo
					SET  arqstatus='I'
					WHERE arqid in (SELECT
										arqid
									FROM
										projovemurbano.arquivo_turma
									WHERE
										turid = {$_REQUEST['turid']}
									);";
			$db->executar($sql);
			$db->commit();
		}	
		
		if($_FILES["arquivo"]['name']!=''&& !$_POST['caeid']){
// ver($_REQUEST['turid'],d);
    		require_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	    		 
    		$campos = array("turid" => $_REQUEST['turid']);
	    		 
    		$file = new FilesSimec("arquivo_turma", $campos, 'projovemurbano');

    		$file->setUpload("Arquivo Conselho de Classes: $caecpf");

			$sql = "SELECT arqid FROM projovemurbano.arquivo_turma WHERE turid = {$_REQUEST['turid']}";
			$arqid = $db->pegaUm($sql);
	    }
// 	    ver('sdhaj',d);
	    if($temarquivo && !$_FILES["arquivo"]['name']){
			if($_POST['caeid']){
		    	foreach( $_POST['caeid'] as $caeid ){
		
		    		/*$sqlchecaconcelho = "SELECT true FROM projovemurbano.avaliacaoconselhodeclasse WHERE caeid = $caeid  AND turid = {$_REQUEST['turid']}";
				    					    			
					$checaconcelho = $db->pegaUm($sqlchecaconcelho);
					ver($sqlchecaconcelho,d);
					if($total['totalpresenca']!= ''||$total['totalnotas'] != ''){
				    					 
						if($checaconcelho == 't'){
		
					    	$sql = "UPDATE projovemurbano.avaliacaoconselhodeclasse
					    			SET  totalpresenca= '1080'
					    				,totalnotas= '1100'
					    			WHERE caeid = $caeid AND turid = {$_REQUEST['turid']};";
				    						// 		    				ver($sql,d);
				    		$db->executar($sql);
				    		$db->commit();
				    	}else{*/
				    		$sql = "INSERT INTO projovemurbano.avaliacaoconselhodeclasse(
				    				caeid, turid, totalpresenca, totalnotas)
				    				VALUES ($caeid, {$_REQUEST['turid']},
				    				'1080',
				    				'1100')";
				    			// 		    				ver($sql,d);
				    		$db->executar($sql);
				    		$db->commit();
		//     			}
				   
		// 			}
				}
			}else{
				echo "<script>
				alert('Para salvar modifique pelo menos um aluno.');
				window.location = 'projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=relatorio_acompanhamento_freq&acao=A&polid=".$_REQUEST['polid']."&nucid=".$_REQUEST['nucid']."&turid=".$_REQUEST['turid']."';
				</script>";
				die;
				
			}
		}
	echo "<script>
			alert('Operação realizada com sucesso.');
			window.location = 'projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=relatorio_acompanhamento_freq&acao=A&polid=".$_REQUEST['polid']."&nucid=".$_REQUEST['nucid']."&turid=".$_REQUEST['turid']."';
			</script>";
}

/*Fim Funções da aba Acompanhamneto de Frequência e Nota*/

function retornaDadosDiario( $turid, $periodo ){

	global $db;

	$sqlDadosDiario = "SELECT distinct dia.diaid
                    FROM projovemurbano.diariofrequencia dif
                    INNER JOIN projovemurbano.diario dia
                        ON dif.diaid = dia.diaid
                    INNER JOIN workflow.documento doc
                        ON dia.docid = doc.docid                    
                    INNER JOIN projovemurbano.gradecurricular grd
                        ON dif.grdid = grd.grdid
                    INNER JOIN projovemurbano.componentecurricular coc
                        ON grd.cocid = coc.cocid
                    INNER JOIN projovemurbano.periodocurso per
                        ON dia.perid = per.perid
                    INNER JOIN projovemurbano.unidadeformativa unf
                        ON per.unfid = unf.unfid
                    INNER JOIN projovemurbano.ciclocurso cic
                        ON unf.cicid = cic.cicid
                    INNER JOIN projovemurbano.turma tur
                        ON dia.turid = tur.turid
                    INNER JOIN projovemurbano.nucleo nuc
                        ON tur.nucid = nuc.nucid
                    LEFT JOIN projovemurbano.nucleoescola nes
                        ON nuc.nucid = nes.nucid
                        AND nes.nuetipo = 'S' 
                    LEFT JOIN entidade.entidade ent
                        ON nes.entid = ent.entid
                    LEFT JOIN entidade.endereco ende
                        ON ent.entid = ende.entid
                    LEFT OUTER JOIN municipio pmun 
                        ON ende.muncod = pmun.muncod 
                    WHERE coc.cocdisciplina   = 'D'
                    and per.perid = {$periodo}
                    AND dia.turid           = {$turid}
                   ";             
	 
	$dadosDiario = $db->pegaLinha($sqlDadosDiario);

	return $dadosDiario;
}


if($_REQUEST['pjuid']) $_SESSION['projovemurbano']['pjuid']=$_REQUEST['pjuid'];
if(!isset($_SESSION['projovemurbano']['muncod'])||!isset($_SESSION['projovemurbano']['estuf'])){ carregarProJovemUrbanoUF_MUNCOD(); }
if(!$_SESSION['projovemurbano']['pjuid'])die("<script>alert('Problemas de navegação. Inicie novamente.');window.location='projovemurbano.php?modulo=inicio&acao=C';</script>");

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	//buscarTurmas2();
	exit;
}

$docid = $db->pegaUm("SELECT docid FROM projovemurbano.projovemurbano WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'");

if(!$docid) {
	$docid = criaDocumento();
	$db->executar("UPDATE projovemurbano.projovemurbano SET docid='".$docid."' WHERE pjuid='".$_SESSION['projovemurbano']['pjuid']."'");
	$db->commit();
}

$esdid = $db->pegaUm("SELECT esdid FROM workflow.documento WHERE docid='".$docid."'");

include_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br>';

echo montarAbasArray(montaMenuProJovemUrbano(), "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A");

monta_titulo('Projovem Urbano', montaTituloEstMun());

switch( $_GET['aba'] )
{
	case 'cadastroEstudantes':
		$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=cadastroEstudantes";
		$pagAtiva = "cadastroEstudantes.inc";
		break;
	case 'diarioFrequencia':
		$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=diarioFrequencia";
		$pagAtiva = "diarioFrequencia.inc";
		break;
	case 'frequenciaMensal':
		$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=frequenciaMensal";
		$pagAtiva = "frequenciaMensal.inc";
		break;

	case 'trabalhoMensal':
		$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=trabalhoMensal";
		$pagAtiva = "trabalhoMensal.inc";
		break;

	case 'agencias':
		$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=agencias";
		$pagAtiva = "agencias.inc";
		break;

	case 'lancamentoNotas':
		$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=lancamentoNotas";
		$pagAtiva = "lancamentoNotas.inc";
		break;
            
	case 'encaminharLista':
		$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=encaminharLista";
		$pagAtiva = "encaminharLista.inc";
		break;
		
	case 'relatorio_acompanhamento_freq':
		$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=relatorio_acompanhamento_freq";
		$pagAtiva = "relatorio_acompanhamento_freq.inc";
		break;

	default:
		$perfis = pegaPerfilGeral();
		if( in_array(PFL_COORDENADOR_MUNICIPAL, $perfis) || in_array(PFL_COORDENADOR_ESTADUAL, $perfis) )
		{
			$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=agencias";
			$pagAtiva = "agencias.inc";

		}elseif( in_array(PFL_DIRETOR_POLO, $perfis) ){

			$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=encaminharLista";
			$pagAtiva = "encaminharLista.inc";

		}else{
			$abaAtiva = "/projovemurbano/projovemurbano.php?modulo=principal/monitoramento&acao=A&aba=cadastroEstudantes";
			$pagAtiva = "cadastroEstudantes.inc";
		}
		break;
}

echo "<br>";

//if(!$db->testa_superuser()){
//	echo '<script>document.getElementById(\'aguarde\').style.display = \'none\';</script>
//		  <table border="0" cellpadding="0" cellspacing="0" width="95%" align="center">
//			<tbody>
//				<tr bgcolor="#294054">
//			    	<td height="40"><img src="../manutencao_arquivos/top-a.gif" border="0" height="42" width="370"></td>
//			    	<td align="right" height="40"><img src="../manutencao_arquivos/top-b.jpg" border="0" height="42" width="430"></td>
//				</tr>
//				<tr bgcolor="#cccccc">
//			    	<td colspan="2" height="1"></td>
//				</tr>
//			    <tr bgcolor="#336633">
//			    	<td colspan="2" height="5"></td>
//				</tr>
//			    <tr bgcolor="#c0c0c0">
//			    	<td colspan="2" height="2"></td>
//				</tr>
//			    <tr>
//			    	<td colspan="2" height="2" style="padding: 100px">
//
//		<p style="margin: 0 0 0 0;text-align: center; color: red; font-size: 30px">Página em manutenção.</p>
//		</td>
//				</tr>
//		<tr height="20">
//				<td bgcolor="#2a4159" align="left">
//					<font color="#ffffff">  Data do Sistema: - </font>
//				</td>
//				<td bgcolor="#2a4159" align="right">
//					<font color="#ffffff">SIMEC - Ministério da Educação  </font>
//				</td>
//		</tr>
//			</tbody>
//		</table>';
//	die();
//}

echo montarAbasArray(montaMenuMonitoramento(), $abaAtiva);

if ( !empty($pagAtiva) ){
	include $pagAtiva;
}
?>
