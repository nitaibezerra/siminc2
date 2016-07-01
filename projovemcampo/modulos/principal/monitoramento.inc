<?php
//ver(,d);
function carregaOcupacao($request) {
  global $db;
  $sql = "SELECT ocuid as codigo, ocudesc as descricao FROM projovemcampo.ocupacao WHERE ocustatus='A' ORDER BY ocudesc";
  $dados = $db->carregar($sql);?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
  <?php foreach($dados as $dado): ?>
  <tr>
    <td><?=$dado['descricao']; ?></td>
  </tr>
  <?php endforeach; ?>
</table>
<?php
}

function carregaProgramasBeneficiarios($request) {
  global $db;
  $sql = "SELECT pbeid as codigo, pbedesc as descricao FROM projovemcampo.programabeneficiario WHERE pbestatus='A' ORDER BY pbedesc";
  $dados = $db->carregar($sql); ?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
  <?php foreach($dados as $dado): ?>
  <tr>
    <td><?=$dado['descricao'] ?></td>
  </tr>
  <?php endforeach; ?>
</table>
<?php
}


function carregaDeficiencia($request) {
  global $db;
  $sql = "SELECT tdeid as codigo, tdedesc as descricao FROM projovemcampo.tipodeficiencia WHERE tdestatus='A' ORDER BY tdedesc";
  $dados = $db->carregar($sql); ?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
  <?php foreach($dados as $dado): ?>
  <tr>
    <td><?=$dado['descricao']; ?></td>
  </tr>
  <?php endforeach; ?>
</table>
<?php
}

function carregaRaca($request) {
  global $db;
  $sql = "SELECT craid as codigo, cradesc as descricao FROM projovemcampo.corraca WHERE crastatus='A' ORDER BY cradesc";
  $dados = $db->carregar($sql); ?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
  <?php foreach($dados as $dado): ?>
  <tr>
    <td><?=$dado['descricao'] ?></td>
  </tr>
  <?php endforeach; ?>
</table>
<?php
}

function carregaNacionalidade( $request ){
  $naturalidade = array(
      0 => array(
          "codigo" => "B",
          "descricao" => "Brasileira"),
      1 => array(
          "codigo" => "E",
          "descricao" => "Estrangeira"));
  $dados = $naturalidade; ?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
  <?php foreach($dados as $dado): ?>
  <tr>
    <td><?=$dado['descricao']; ?></td>
  </tr>
  <?php endforeach; ?>
</table>
<?php
}

function carregaTranstorno($request){
  global $db;
  $sql = "SELECT tdiid as codigo, tdidesc as descricao FROM projovemcampo.transdesininfancia WHERE tdistatus='A' AND tdiid not in (5) ORDER BY tdidesc";
  $dados = $db->carregar($sql); ?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
  <?php foreach($dados as $dado): ?>
  <tr>
    <td><?=$dado['descricao']; ?></td>
  </tr>
  <?php endforeach; ?>
</table>
<?php
}

function carregaEstadoCivil($request){
  global $db;
  $sql = "SELECT escid as codigo, escdesc as descricao FROM projovemcampo.estadocivil WHERE escstatus='A' ORDER BY escdesc";
  $dados = $db->carregar($sql); ?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
  <?php foreach($dados as $dado): ?>
  <tr>
    <td><?=$dado['descricao']; ?></td>
  </tr>
  <?php endforeach; ?>
</table>
<?php
}

function carregaSegmentoSocial($request) {
  global $db;
  $sql = "SELECT ssoid as codigo, ssodesc as descricao FROM projovemcampo.segmentosocial WHERE ssostatus='A' ORDER BY ssodesc";
  $dados = $db->carregar($sql); ?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
  <?php foreach($dados as $dado): ?>
  <tr>
    <td><?=$dado['descricao'] ?></td>
  </tr>
  <?php endforeach; ?>
</table>
<?php
}

function carregaUF($request) {
  global $db;
  $sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
  $dados = $db->carregar($sql); ?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
  <?php foreach($dados as $dado): ?>
  <tr>
      <td><?=$dado['descricao']; ?></td>
  </tr>
  <?php endforeach; ?>
</table>
<?php
}
function carrega_Municipio($request) {
  global $db;
  //$sql = "SELECT estuf, muncod, mundescricao as mundsc FROM territorios.municipio WHERE estuf = '" . $request['endestuf'] . "' ORDER BY mundescricao municipio";
  $sql = "SELECT estuf, muncod, mundescricao as mundsc FROM territorios.municipio WHERE estuf = 'MG' ORDER BY mundescricao LIMIT 20";
  $dados = $db->carregar($sql); ?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
  <?php foreach($dados as $dado): ?>
  <tr>
    <td><?=$dado['mundsc']; ?></td>
  </tr>
  <?php endforeach; ?>
</table>
<?php
}

function carregaNucleoEscola( $request ){
	global $db;

	// Adaptação para o perfil Diretor do Núcleo
	if(!$db->testa_superuser()) {
		$perfis = pegaPerfilGeral();
		if(in_array(PFL_DIRETOR_NUCLEO, $perfis)) {
			$inner_nucleo = "inner join projovemcampo.usuarioresponsabilidade ur on ur.usucpf='".$_SESSION['usucpf']."' and ur.entid=nes.entid AND rpustatus='A'";
		}
	}

	if($request['possuipolo']=="t") {

		$sql = "SELECT DISTINCT
					nuc.nucid as codigo, 
					'NÚCLEO '||nuc.nucid||', SEDE: '||(SELECT entnome FROM entidade.entidade ent INNER JOIN projovemcampo.nucleoescola nes ON nes.entid = ent.entid WHERE nes.nucid=nuc.nucid AND nes.nuetipo='S')||COALESCE(', ANEXO: '||(SELECT entnome FROM entidade.entidade ent INNER JOIN projovemcampo.nucleoescola nes ON nes.entid = ent.entid WHERE nes.nucid=nuc.nucid AND nes.nuetipo='A'),'') as descricao 
				FROM projovemcampo.nucleo nuc
				INNER JOIN projovemcampo.nucleoescola nes ON nes.nucid = nuc.nucid
				INNER JOIN projovemcampo.municipio mun ON mun.munid = nuc.munid 
				INNER JOIN projovemcampo.associamucipiopolo amp ON amp.munid = mun.munid    
				INNER JOIN projovemcampo.polo pol ON pol.polid = amp.polid 
				INNER JOIN projovemcampo.polomunicipio plm ON plm.pmuid = pol.pmuid 
				{$inner_nucleo}
				WHERE 
					nuc.nucstatus='A' AND mun.munstatus='A' AND plm.pmustatus='A' 
					/*Retirado a pedido do Wallace - 08/05/2012*/
				  	/*AND nuc.nucid NOT IN (SELECT
				  							nuc2.nucid
				  						  FROM
				  						  	projovemcampo.nucleo nuc2
				  						  WHERE
				  						  	nuc2.nucqtdestudantes <= ( SELECT count(caeid) FROM projovemcampo.cadastroestudante cae WHERE cae.nucid = nuc2.nucid  ))*/
				  	AND pol.polstatus='A' ".(($_SESSION['projovemcampo']['apcid'])?" AND apcid='".$_SESSION['projovemcampo']['apcid']."'":"").(($polid)?" AND pol.polid='".$polid."'":"");
				$nucleos = $db->carregar($sql);

	} else {

		$sql = "SELECT DISTINCT
					nuc.nucid as codigo, 
					'NÚCLEO '||nuc.nucid||', 
					SEDE: '||(SELECT entnome FROM entidade.entidade ent INNER JOIN projovemcampo.nucleoescola nes ON nes.entid = ent.entid WHERE nes.nucid=nuc.nucid AND nes.nuetipo='S')||
					COALESCE(', ANEXO:'||(SELECT entnome FROM entidade.entidade ent INNER JOIN projovemcampo.nucleoescola nes ON nes.entid = ent.entid WHERE nes.nucid=nuc.nucid AND nes.nuetipo='A'),'') as descricao 
				FROM 
					projovemcampo.nucleo nuc 
				INNER JOIN projovemcampo.nucleoescola nes ON nes.nucid = nuc.nucid
			    INNER JOIN projovemcampo.municipio mun ON mun.munid = nuc.munid 
			    INNER JOIN projovemcampo.polomunicipio plm ON plm.pmuid = mun.pmuid 
			    {$inner_nucleo}
			    WHERE 
			  		nuc.nucstatus='A' AND mun.munstatus='A' AND plm.pmustatus='A'
			  		/*Retirado a pedido do Wallace - 08/05/2012*/
			  		/*AND nuc.nucid NOT IN (SELECT
				  							nuc2.nucid
				  						  FROM
				  						  	projovemcampo.nucleo nuc2
				  						  WHERE
				  						  	nuc2.nucqtdestudantes <= ( SELECT count(caeid) FROM projovemcampo.cadastroestudante cae WHERE cae.nucid = nuc2.nucid  ))*/ 
			  		AND apcid='".$_SESSION['projovemcampo']['apcid']."'";
			    $nucleos = $db->carregar($sql);

	}
	//ver( $sql );
	//return $nucleos;
	$dados = $db->carregar($sql);
	?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
  <?php foreach($dados as $dado): ?>
  <tr>
    <td><?=$dado['descricao']; ?></td>
  </tr>
  <?php endforeach; ?>
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
				if( $_SESSION['projovemcampo']['entid'] ){
					$escola_diretor = "t.entid = ".$_SESSION['projovemcampo']['entid']." AND ";
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
         ', Total de Alunos: '||(SELECT count(*) FROM projovemcampo.cadastroestudante c WHERE c.turid = t.turid AND caestatus = 'A') as descricao
         FROM
         projovemcampo.turma t
         INNER JOIN projovemcampo.nucleoescola nes ON nes.entid = t.entid AND nes.nucid = ".$request['nucid']."
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
                                    ', Total de Alunos: '||(SELECT count(*) FROM projovemcampo.cadastroestudante c WHERE c.turid = t.turid AND caestatus = 'A') as descricao
                                    FROM
                                    projovemcampo.turma t
                                    INNER JOIN projovemcampo.nucleoescola nes ON nes.entid = t.entid AND nes.nucid = 1417
                                    WHERE

                                   t.nucid = 1417 AND turstatus = 'A'
                                   ORDER BY
                                   2
                           ";
		$dados = $db->carregar($sql);
	}
	?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
  <?php foreach($dados as $dado): ?>
    <tr>
      <td><?=$dado['descricao']; ?></td>
    </tr>
    <?php endforeach; ?>
</table>
<?php
}

if (!empty($_REQUEST['testaDados'])) {
  $retorno = '';

  if ($_REQUEST['testaDados']($_REQUEST)) {
    $retorno = 1;
  }
  echo $retorno;
  die();
}


function verificarFaltasAluno($dados) {
  global $db;

  $turid = $dados['turid'];
  $nucid = (int) $dados['nucid'];

  $parametros = array(
      'turid' => $turid,
      'nucid' => $nucid );

  $periodoAtual = retornaPeriodoAtual($turid);
if( is_array( $periodoAtual ) ){
	foreach ($periodoAtual as $periodo) {
    	$dadosDiario = retornaDadosDiario($turid, $periodo['periodo']);

    foreach ($dadosDiario as $diario) {
	    $componentesCurriculares  = listarComponenteCurricular($dadosDiario['diaid']);
	    $qtdColunas = count($componentesCurriculares);

    for ($i = 0; $i < $qtdColunas; $i++) {
        $parametrosPresenca = array(
            'difid' => $componentesCurriculares[$i]['difid'],
            'caeid' => $_REQUEST['caeid']);
        $dadosPresenca = listaPresencaPorAlunoDiarioFechado($parametrosPresenca);
        $qtdPresenca     = (!empty($dadosPresenca['frqqtdpresenca'])?$dadosPresenca['frqqtdpresenca']:'0');
        $qtdAulaDada     = (!empty($dadosPresenca['difqtdauladada'])?$dadosPresenca['difqtdauladada']:'0');
//				$somaQuantidadePresenca += $qtdPresenca;
//				$somaQuantidadeAulaDada += $qtdAulaDada;
        $tot += ( $qtdAulaDada - $qtdPresenca );
      }
    }
  }
}  
//	$qtdHorasMaxima = (int)( $somaQuantidadeAulaDada );
//	$qtdHorasMaximaOld = (int)( count( $periodoAtual) * 80 );
//	$qtdHorasFalta =  (int)( $qtdHorasMaxima - $somaQuantidadePresenca );
//	$qtdHorasFaltaOld =  (int)( $qtdHorasMaximaOld - $somaQuantidadePresenca );
//	if( $qtdHorasFalta > 360 ){
  if ($tot > 360) {
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
			FROM projovemcampo.frequenciaestudante frq 
			INNER JOIN projovemcampo.diariofrequencia dif 
				ON frq.difid = dif.difid 
			INNER JOIN projovemcampo.gradecurricular grd 
				ON dif.grdid = grd.grdid 
			INNER JOIN projovemcampo.componentecurricular coc 
				ON grd.cocid = coc.cocid 
			INNER JOIN projovemcampo.diario as dia ON dif.diaid = dia.diaid 
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
// 	if($turid < 1){
// 		return false;
// 	}
// 	$sql = "
// 		SELECT distinct dia.perid as periodo
// 		        	FROM projovemcampo.diario dia
// 		        	INNER JOIN projovemcampo.periodocurso as per ON dia.perid = per.perid
// 		        	INNER JOIN projovemcampo.diariofrequencia as diaf on diaf.diaid = dia.diaid
// 		        	INNER JOIN projovemcampo.frequenciaestudante as freq on freq.difid = diaf.difid
// 		        	INNER JOIN workflow.documento as doc
// 				        ON doc.docid = dia.docid
// 		and dia.turid = $turid and doc.esdid = ".WF_ESTADO_DIARIO_FECHADO." order by periodo";

// 	$periodo = $db->carregar( $sql );

// 	return $periodo;
}

if ($_REQUEST['reqTittle']) {
  $_REQUEST['reqTittle']($_REQUEST);
  die();
}

function retornaDadosDiario($turid, $periodo) {
  global $db;
  $sqlDadosDiario = <<<DML
SELECT distinct dia.diaid
  FROM projovemcampo.diariofrequencia dif
    INNER JOIN projovemcampo.diario dia
      ON dif.diaid = dia.diaid
    INNER JOIN workflow.documento doc
      ON dia.docid = doc.docid                    
    INNER JOIN projovemcampo.gradecurricular grd
      ON dif.grdid = grd.grdid
    INNER JOIN projovemcampo.componentecurricular coc
      ON grd.cocid = coc.cocid
    INNER JOIN projovemcampo.periodocurso per
      ON dia.perid = per.perid
    INNER JOIN projovemcampo.unidadeformativa unf
      ON per.unfid = unf.unfid
    INNER JOIN projovemcampo.ciclocurso cic
      ON unf.cicid = cic.cicid
    INNER JOIN projovemcampo.turma tur
      ON dia.turid = tur.turid
    INNER JOIN projovemcampo.nucleo nuc
      ON tur.nucid = nuc.nucid
    LEFT JOIN projovemcampo.nucleoescola nes
      ON nuc.nucid = nes.nucid
          AND nes.nuetipo = 'S' 
    LEFT JOIN entidade.entidade ent
      ON nes.entid = ent.entid
    LEFT JOIN entidade.endereco ende
      ON ent.entid = ende.entid
    LEFT OUTER JOIN municipio pmun 
      ON ende.muncod = pmun.muncod 
  WHERE coc.cocdisciplina = 'D'
    AND per.perid = {$periodo}
    AND dia.turid = {$turid}
DML;

  $dadosDiario = $db->pegaLinha($sqlDadosDiario);
  return $dadosDiario;
}

if ($_REQUEST['apcid']) {
  $_SESSION['projovemcampo']['apcid']=$_REQUEST['apcid'];
}
if (!isset($_SESSION['projovemcampo']['muncod'])
        || !isset($_SESSION['projovemcampo']['estuf'])) {
  carregarprojovemcampoUF_MUNCOD();
}
if (!$_SESSION['projovemcampo']['apcid']) {
  die("<script>alert('Problemas de navegação. Inicie novamente.');window.location='projovemcampo.php?modulo=inicio&acao=C';</script>");
}
if ($_REQUEST['requisicao']) {
  $_REQUEST['requisicao']($_REQUEST);
  //buscarTurmas2();
//  ver($_REQUEST['requisicao']);
  exit;
}

$docid = $db->pegaUm("SELECT docid FROM projovemcampo.adesaoprojovemcampo WHERE apcid='".$_SESSION['projovemcampo']['apcid']."'");

if (!$docid) {
  $docid = criaDocumento();
  $db->executar("UPDATE projovemcampo.adesaoprojovemcampo SET docid='".$docid."' WHERE apcid='".$_SESSION['projovemcampo']['apcid']."'");
  $db->commit();
}

$esdid = $db->pegaUm("SELECT esdid FROM workflow.documento WHERE docid='".$docid."'");

include_once APPRAIZ . 'includes/cabecalho.inc';
echo '<br>';
echo montarAbasArray(montaMenuprojovemcampo(), "/projovemcampo/projovemcampo.php?modulo=principal/monitoramento&acao=A");

monta_titulo('Projovem Campo', montaTituloEstMun());
switch ($_GET['aba']) {
  case 'cadastroEstudantes':
    $abaAtiva = "/projovemcampo/projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=cadastroEstudantes";
    $pagAtiva = "monitoramento/cadastroEstudantes.inc";
    break;
  case 'alteraRangeDiario':
    $abaAtiva = "/projovemcampo/projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=alteraRangeDiario";
    $pagAtiva = "monitoramento/alteraRangeDiario.inc";
    break;
  case 'diarioFrequencia':
    $abaAtiva = "/projovemcampo/projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=diarioFrequencia";
    $pagAtiva = "monitoramento/diarioFrequencia.inc";
    break;
  case 'frequenciaMensal':
    $abaAtiva = "/projovemcampo/projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=frequenciaMensal";
    $pagAtiva = "monitoramento/frequenciaMensal.inc";
    break;
  case 'agencias':
    $abaAtiva = "/projovemcampo/projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=agencias";
    $pagAtiva = "monitoramento/agencias.inc";
    break;
  case 'encaminharLista':
    $abaAtiva = "/projovemcampo/projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=encaminharLista";
    $pagAtiva = "monitoramento/encaminharLista.inc";
    break;
  default:
      $abaAtiva = "/projovemcampo/projovemcampo.php?modulo=principal/monitoramento&acao=A&aba=cadastroEstudantes";
      $pagAtiva = "monitoramento/cadastroEstudantes.inc";
   break;
}
echo "<br>";
echo montarAbasArray(montaMenuMonitoramento(), $abaAtiva);


if (!empty($pagAtiva)) {
  include $pagAtiva;
}