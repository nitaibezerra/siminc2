<?php
header( 'Content-Type: text/html; charset=ISO-8859-1' );
//header( 'Content-Type: text/html; charset=UTF-8' );

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );


error_reporting( E_ALL ^ E_NOTICE );

ini_set("memory_limit", "1024M");
set_time_limit(0);

ini_set( 'soap.wsdl_cache_enabled', '0' );
ini_set( 'soap.wsdl_cache_ttl', 0 );


$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
require_once APPRAIZ . "includes/workflow.php";
require_once APPRAIZ . "www/sispacto/_constantes.php";
require_once APPRAIZ . "www/sispacto/_funcoes.php";


if(!$_SESSION['usucpf']) {
	// CPF do administrador de sistemas
	$_SESSION['usucpforigem'] = '00000000191';
	$_SESSION['usucpf'] = '00000000191';
}
    
   
// abre conexção com o servidor de banco de dados
$db = new cls_banco();

if(!$_REQUEST['somentesincronizar']) {
	
	$sql = "update sispacto.identificacaousuario set iusdesativado=true where iusd in(
			select i.iusd from sispacto.identificacaousuario i 
			inner join sispacto.tipoperfil t on t.iusd = i.iusd and t.pflcod in(827,849,826,848,847,846)
			where i.iusdesativado!=true
			)";
	
	$db->executar($sql);
	$db->commit();
	
	
	$sql = "update sispacto.gestaomobilizacaoperguntas x set gmpnaoatividades=true from (
			select * from (
			select gmpid, (select count(*) from sispacto.gestaomobilizacaocoordenadorlocal where gmcstatus='A' AND iusd=i.iusd) as tt from sispacto.gestaomobilizacaoperguntas i where gmpnaoatividades=false
			) foo where foo.tt=0
			) foo2 where foo2.gmpid = x.gmpid";
	
	$db->executar($sql);
	$db->commit();
	
	
	$sql = "update sispacto.identificacaousuario x set cadastradosgb=false from (
	select i.iusd, i.iuscpf, i.iusnome, i.iusagenciasugerida, s.agencia, i.cadastradosgb from sispacto.bolsistaserroagencia s 
	inner join sispacto.identificacaousuario i on i.iuscpf = trim(s.cpf)
	inner join sispacto.tipoperfil t on t.iusd = i.iusd 
	where i.iusagenciasugerida != s.agencia ) foo where foo.iusd = x.iusd";
	
	$db->executar($sql);
	$db->commit();
	
	$sql = "update sispacto.identificacaousuario f set iustipoprofessor=foo.tipop from (
	
	select i.iusd, i.iuscpf, i.iustipoprofessor, case when p.cpf is null then 'cpflivre' else 'censo' end as tipop from sispacto.identificacaousuario i 
	inner join sispacto.tipoperfil t on t.iusd = i.iusd and pflcod=849
	left join sispacto.professoresalfabetizadores p on p.cpf = i.iuscpf 
	where i.iustipoprofessor != case when p.cpf is null then 'cpflivre' else 'censo' end or i.iustipoprofessor is null
	
	) foo where foo.iusd = f.iusd";
	
	$db->executar($sql);
	$db->commit();
	
	
	$sql = "update sispacto.turmasprofessoresalfabetizadores t set tpatotalmeninos=null, tpatotalmeninas=null from (
	select tpaid
	from sispacto.turmasprofessoresalfabetizadores t
	WHERE (tpatotalmeninos is not null or tpatotalmeninas is not null) and (COALESCE(tpafaixaetariaacima11anos,0)+COALESCE(tpafaixaetaria5anos,0)+COALESCE(tpafaixaetaria6anos,0)+COALESCE(tpafaixaetaria7anos,0)+COALESCE(tpafaixaetaria8anos,0)+COALESCE(tpafaixaetaria9anos,0)+COALESCE(tpafaixaetaria10anos,0)+COALESCE(tpafaixaetaria11anos,0))=0
	) foo where t.tpaid = foo.tpaid
	";
	
	$db->executar($sql);
	$db->commit();
	
	
	$sql = "update sispacto.identificacaousuario x set muncodatuacao=foo.muncodatuacao from (
	select i.iusd, i2.muncodatuacao from sispacto.identificacaousuario i 
	inner join sispacto.tipoperfil t on t.iusd = i.iusd and t.pflcod=849
	inner join sispacto.orientadorturma o on o.iusd = i.iusd 
	inner join sispacto.turmas tu on tu.turid = o.turid 
	inner join sispacto.identificacaousuario i2 on i2.iusd = tu.iusd 
	inner join sispacto.tipoperfil t2 on t2.iusd = i2.iusd and t2.pflcod=827
	where i.iusstatus='A' and i2.iusstatus='A' and i2.iusformacaoinicialorientador=true and i.muncodatuacao is null and i2.muncodatuacao is not null) foo
	where x.iusd = foo.iusd";
	
	$db->executar($sql);
	$db->commit();
	
	
	$sql = "
	delete from sispacto.historicoreaberturanota where mavid in(
	
	select distinct ma.mavid from sispacto.mensario m 
	inner join sispacto.mensarioavaliacoes ma on ma.menid = m.menid 
	inner join sispacto.tipoperfil t on t.iusd = m.iusd and t.pflcod=849 
	left join sispacto.tipoperfil t2 on t2.iusd = ma.iusdavaliador 
	left join sispacto.turmas tu on tu.iusd = t2.iusd 
	left join sispacto.orientadorturma ot on ot.turid = tu.turid and ot.iusd = t.iusd 
	inner join (
	
	select m.iusd, m.fpbid, count(*) from sispacto.mensario m 
	inner join sispacto.mensarioavaliacoes ma on ma.menid = m.menid 
	inner join sispacto.tipoperfil t on t.iusd = m.iusd and t.pflcod=849 
	group by m.iusd,  m.fpbid 
	having count(*)>1
	
	) foo on t.iusd=foo.iusd and m.fpbid=foo.fpbid 
	
	)";
	
	$db->executar($sql);
	$db->commit();
	
	$sql = "delete from sispacto.mensarioavaliacoes where mavid in(
	select distinct ma.mavid from sispacto.mensario m 
	inner join sispacto.mensarioavaliacoes ma on ma.menid = m.menid 
	inner join sispacto.tipoperfil t on t.iusd = m.iusd and t.pflcod=849 
	left join sispacto.tipoperfil t2 on t2.iusd = ma.iusdavaliador 
	left join sispacto.turmas tu on tu.iusd = t2.iusd 
	left join sispacto.orientadorturma ot on ot.turid = tu.turid and ot.iusd = t.iusd 
	inner join (
	
	select m.iusd, m.fpbid, count(*) from sispacto.mensario m 
	inner join sispacto.mensarioavaliacoes ma on ma.menid = m.menid 
	inner join sispacto.tipoperfil t on t.iusd = m.iusd and t.pflcod=849 
	group by m.iusd,  m.fpbid 
	having count(*)>1
	
	) foo on t.iusd=foo.iusd and m.fpbid=foo.fpbid 
	where ot.otuid is null
	)";
	
	$db->executar($sql);
	$db->commit();
	
	
	$sql = "delete from sispacto.historicoreaberturanota where mavid in(
	select ma.mavid from sispacto.mensario m 
	inner join sispacto.mensarioavaliacoes ma on ma.menid = m.menid 
	inner join workflow.documento d on d.docid = m.docid 
	inner join workflow.estadodocumento e on e.esdid = d.esdid
	where mavfrequencia is null and mavatividadesrealizadas is null and mavmonitoramento is null and mavrecomendadocertificacao is null
	and e.esdid!=657
	)";
	
	$db->executar($sql);
	$db->commit();
	
	$sql = "delete from sispacto.mensarioavaliacoes where mavid in(
	select ma.mavid from sispacto.mensario m 
	inner join sispacto.mensarioavaliacoes ma on ma.menid = m.menid 
	inner join workflow.documento d on d.docid = m.docid 
	inner join workflow.estadodocumento e on e.esdid = d.esdid
	where mavfrequencia is null and mavatividadesrealizadas is null and mavmonitoramento is null and mavrecomendadocertificacao is null
	and e.esdid!=657
	)";
	
	$db->executar($sql);
	$db->commit();
	
	
	// CORRIGINDO MENSARIO AVALIADOS INCORRETAMENTE 
	// PROFESSORES COM ALGUMA AVALIAÇÃO ZERADA FEITA POR FORMADOR
	
	$sql = "DELETE FROM sispacto.historicoreaberturanota WHERE mavid in(
			select ma.mavid from sispacto.mensario m 
			inner join sispacto.identificacaousuario i on i.iusd = m.iusd 
			inner join sispacto.tipoperfil t on t.iusd = i.iusd and t.pflcod = 849 
			inner join sispacto.mensarioavaliacoes ma on ma.menid = m.menid 
			inner join sispacto.identificacaousuario i2 on i2.iusd = ma.iusdavaliador 
			inner join sispacto.tipoperfil t2 on t2.iusd = i2.iusd and t2.pflcod = 848 
			where mavfrequencia=0 and mavatividadesrealizadas=0 and mavmonitoramento=0 and mavtotal=0 and mavrecomendadocertificacao is null
			)";
	
	$db->executar($sql);
	$db->commit();
	
	
	$sql = "DELETE FROM sispacto.mensarioavaliacoes WHERE mavid IN(
			SELECT ma.mavid FROM sispacto.mensario m 
			INNER JOIN sispacto.identificacaousuario i on i.iusd = m.iusd 
			INNER JOIN sispacto.tipoperfil t on t.iusd = i.iusd and t.pflcod = 849 
			INNER JOIN sispacto.mensarioavaliacoes ma on ma.menid = m.menid 
			INNER JOIN sispacto.identificacaousuario i2 on i2.iusd = ma.iusdavaliador 
			INNER JOIN sispacto.tipoperfil t2 on t2.iusd = i2.iusd and t2.pflcod = 848 
			WHERE mavfrequencia=0 AND mavatividadesrealizadas=0 AND mavmonitoramento=0 AND mavtotal=0 and mavrecomendadocertificacao is null
			)";
	
	$db->executar($sql);
	$db->commit();
	
	$sql = "UPDATE sispacto.usuarioresponsabilidade x SET uncid=xx.uncid FROM (
	
			SELECT * from (
			SELECT i.uncid, (SELECT uncid FROM sispacto.usuarioresponsabilidade WHERE usucpf=i.iuscpf AND pflcod=t.pflcod AND uncid is not null AND rpustatus='A' LIMIT 1) as uncid2, i.iuscpf, t.pflcod FROM sispacto.identificacaousuario i 
			INNER JOIN sispacto.tipoperfil t ON t.iusd = i.iusd 
			) foo WHERE foo.uncid!=foo.uncid2
			
			) xx WHERE x.usucpf=xx.iuscpf AND x.pflcod=xx.pflcod AND rpustatus='A'";
	
	$db->executar($sql);
	$db->commit();
	
	$sql = "update seguranca.usuario x set usunome=foo.iusnome from (
	
	select usucpf, iusnome, trim(replace(removeacento(usunome), 'Ç','C')) from sispacto.identificacaousuario i 
	inner join seguranca.usuario u on u.usucpf = i.iuscpf 
	where trim(iusnome) != trim(replace(removeacento(usunome), 'Ç','C')) and cadastradosgb=true
	
	) foo where x.usucpf=foo.usucpf";
	
	$db->executar($sql);
	$db->commit();
	
	$sql = "update sispacto.identificacaousuario x set uncid=xx.ecu from (
	
	select i.uncid as ius, e.uncid as ecu, i.iusd from sispacto.identificacaousuario i 
	inner join sispacto.tipoperfil t on t.iusd = i.iusd
	inner join sispacto.pactoidadecerta p on p.picid = i.picid 
	inner join sispacto.abrangencia a on a.muncod = p.muncod and esfera='M'
	inner join sispacto.estruturacurso e on e.ecuid = a.ecuid 
	where i.uncid is null
	
	) xx where xx.iusd = x.iusd";
	
	$db->executar($sql);
	$db->commit();
	
	$sql = "update sispacto.identificacaousuario x set uncid=xx.ecu from (
	
	select i.uncid as ius, e.uncid as ecu, i.iusd from sispacto.identificacaousuario i 
	inner join sispacto.tipoperfil t on t.iusd = i.iusd
	inner join sispacto.pactoidadecerta p on p.picid = i.picid 
	inner join territorios.municipio m on m.muncod = i.muncodatuacao 
	inner join sispacto.abrangencia a on a.muncod = m.muncod and esfera='E'
	inner join sispacto.estruturacurso e on e.ecuid = a.ecuid 
	where i.uncid is null
	
	) xx where xx.iusd = x.iusd";
	
	$db->executar($sql);
	$db->commit();
	
	$sql = "UPDATE sispacto.identificacaousuario SET iusemailprincipal=replace(iusemailprincipal,'@com','@meudominio.com') WHERE iusemailprincipal ilike '%@com%';";
	$db->executar($sql);
	$db->commit();
	
	$sql = "UPDATE sispacto.identificacaousuario SET iusemailprincipal=replace(iusemailprincipal,'@.','@') WHERE iusemailprincipal ILIKE '%@.%';";
	$db->executar($sql);
	$db->commit();
	
	$sql = "select distinct i.iusd, s.logresponse from sispacto.identificacaousuario i 
	inner join sispacto.tipoperfil t on t.iusd = i.iusd 
	inner join sispacto.logsgb s on s.logcpf = i.iuscpf and s.logservico='gravarDadosBolsista' and s.logerro=true
	where cadastradosgb=false and iustermocompromisso=true and logresponse ilike '%Erro: 00026:%';";
	
	$arr = $db->carregar($sql);
	
	if($arr[0]) {
		foreach($arr as $ar) {
			$sl = explode("(",$ar['logresponse']);
			$sl = explode(")",$sl[1]);
			
			$iusnome_antigo = $db->pegaUm("SELECT iusnome FROM sispacto.identificacaousuario WHERE iusd='".$ar['iusd']."'");
			
			// somente atualizar se os 9 primeiros digitos forem semelhantes
			if(substr(strtoupper($iusnome_antigo),0,9)==substr(strtoupper(trim($sl[0])),0,9)) {
				$sql = "UPDATE sispacto.identificacaousuario SET iusnome='".trim($sl[0])."' WHERE iusd='".$ar['iusd']."'";
				$db->executar($sql);
			}
		}
		$db->commit();
	}

}

// black list
$pularcpf = $db->carregarColuna("SELECT lnscpf FROM sispacto.listanegrasgb");


$sql = "SELECT DISTINCT i.iusd, l.logcpf FROM sispacto.identificacaousuario i 
		LEFT JOIN sispacto.logsgb l ON l.logcpf = i.iuscpf
		WHERE iustermocompromisso=true AND cadastradosgb=false".(($pularcpf)?" AND iuscpf NOT IN('".implode("','",$pularcpf)."')":"")." ORDER BY l.logcpf DESC";
$iusds = $db->carregarColuna($sql);

libxml_use_internal_errors( true );

if($iusds) {
	foreach($iusds as $iusd) {
		
		$lnsid = $db->pegaUm("INSERT INTO sispacto.listanegrasgb(lnscpf) VALUES ((SELECT iuscpf FROM sispacto.identificacaousuario WHERE iusd='".$iusd."')) RETURNING lnsid;");
		$db->commit();
		
		sincronizarDadosUsuarioSGB(array("iusd" => $iusd, "sincronizacao" => true));
		
		$db->executar("DELETE FROM sispacto.listanegrasgb WHERE lnsid='".$lnsid."'");
		$db->commit();
		
	}
}


echo "Sincronizar USUARIOS DO PACTO NO SGB - OK";


$sql = "SELECT uncid FROM sispacto.universidadecadastro WHERE cadastrosgb=false";
$uncids = $db->carregarColuna($sql);

libxml_use_internal_errors( true );

if($uncids) {
	foreach($uncids as $uncid) {
		sincronizarDadosEntidadeSGB(array("uncid" => $uncid));
	}
}


echo "Sincronizar ENTIDADES DO PACTO NO SGB - OK";


/*
 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
 */
require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= "SISPACTO - Sincronizar Usuários SGB";
$mensagem->From 		= $_SESSION['email_sistema'];
$mensagem->AddAddress($_SESSION['email_sistema'], SIGLA_SISTEMA);
$mensagem->Subject = "Sincronizar Usuários SGB";
$mensagem->Body = "Sincronização realizada com sucesso";
$mensagem->IsHTML( true );
$mensagem->Send();
/*
 * FIM
 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
 */

if($_SESSION['usucpf'] == '00000000191') {
	
	unset($_SESSION['usucpf']);
	unset($_SESSION['usucpforigem']);
	
}

?>