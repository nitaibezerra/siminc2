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
require_once APPRAIZ . "www/sismedio/_constantes.php";
require_once APPRAIZ . "www/sismedio/_funcoes.php";


if(!$_SESSION['usucpf']) {
	// CPF do administrador de sistemas
	$_SESSION['usucpforigem'] = '00000000191';
	$_SESSION['usucpf'] = '00000000191';
}
    
   
// abre conexção com o servidor de banco de dados
$db = new cls_banco();

function getmicrotime() {list($usec, $sec) = explode(" ", microtime()); return ((float)$usec + (float)$sec);}

$microtime = getmicrotime();


if(!$_REQUEST['somentesincronizar']) {
	
$sql = "INSERT INTO sismedio.tipoavaliacaoperfil(
            fpbid, pflcod, uncid, tpatipoavaliacao)
select distinct f.fpbid, 1078 as pflcod, f.uncid, 'monitoramentoTextual' as tpatipoavaliacao from sismedio.folhapagamentouniversidade f 
left join sismedio.tipoavaliacaoperfil t on t.fpbid = f.fpbid and t.uncid = f.uncid and t.pflcod = 1078
where rfustatus='I' and f.pflcod=1190 and t.tpaid is null order by uncid, fpbid
";

$db->executar($sql);
$db->commit();
	
$sql = "delete from sismedio.orientadorturma where turid in(
select t.turid from sismedio.identificacaousuario i 
inner join sismedio.turmas t on t.iusd = i.iusd 
where i.iuscpf ilike 'REM%' and i.iusstatus='I'
)";

$db->executar($sql);
$db->commit();

$sql = "
delete from sismedio.turmas where turid in(
select t.turid from sismedio.identificacaousuario i 
inner join sismedio.turmas t on t.iusd = i.iusd 
where i.iuscpf ilike 'REM%' and i.iusstatus='I'
)";

$db->executar($sql);
$db->commit();

	
$sql = "update sismedio.identificacaousuario x set iusnome=foo.nome from (
select iusd, substr(iusnome,0,strpos(iusnome,'<')) as nome from sismedio.identificacaousuario where iusnome ilike '%<%'
) foo where x.iusd = foo.iusd and foo.nome!=''";

$db->executar($sql);
$db->commit();

	
$sql = "update sismedio.turmas x set turdesc=foo2.turdesc from (
select foo.turid, replace(foo.turdesc,foo.nome,foo.iusnome) as turdesc from (
select i.iusnome, 
case when trim(split_part(tu.turdesc, ' - ', 5))!='' then trim(split_part(tu.turdesc, ' - ', 5))
     when trim(split_part(tu.turdesc, ' - ', 4))!='' then trim(split_part(tu.turdesc, ' - ', 4))
     when trim(split_part(tu.turdesc, ' - ', 3))!='' then trim(split_part(tu.turdesc, ' - ', 3))
     when trim(split_part(tu.turdesc, ' - ', 2))!='' then trim(split_part(tu.turdesc, ' - ', 2))
     else 'xx'
end as nome, tu.turid, tu.turdesc


from sismedio.identificacaousuario i 
inner join sismedio.tipoperfil t on t.iusd = i.iusd 
inner join sismedio.turmas tu on tu.iusd = i.iusd 
where i.iusstatus='A'
) foo where foo.nome!=foo.iusnome
) foo2 where foo2.turid=x.turid";

$db->executar($sql);
$db->commit();
	
$sql = "delete from seguranca.perfilusuario p 
using (
select iuscpf, pf.pflcod from sismedio.identificacaousuario i 
inner join seguranca.perfilusuario pp on pp.usucpf = i.iuscpf
inner join seguranca.perfil p on p.pflcod = pp.pflcod and p.sisid=174 
inner join sismedio.pagamentoperfil pf on pf.pflcod = p.pflcod
inner join sismedio.tipoperfil t on t.iusd = i.iusd 
where i.iusstatus='A' and pf.pflcod!=t.pflcod
) foo where p.usucpf=foo.iuscpf and foo.pflcod=p.pflcod
";

$db->executar($sql);
$db->commit();



$sql = "update sismedio.mensario set menencontroqtd=null, menencontrocargahoraria=null  where menencontro=false and (menencontroqtd is not null or menencontrocargahoraria is not null)";


$db->executar($sql);
$db->commit();


$sql = "delete from sismedio.respostasavaliacaocomplementar d
	
	using (
	
	select foo.iusd, foo.fpbid from (
	select m.iusd, m.fpbid, menencontroqtd,menencontrocargahoraria,(select count(*) from sismedio.respostasavaliacaocomplementar where fpbid=m.fpbid and iusdavaliado=m.iusd) as xx, e.esdid, e.esddsc
	from sismedio.mensario m
	inner join workflow.documento d on d.docid = m.docid
	inner join workflow.estadodocumento e on e.esdid = d.esdid
	where menencontro=false and menencontroqtd is null and e.esdid!=951
	) foo where foo.xx>0
	
	) foo2
	where foo2.iusd = d.iusdavaliado and foo2.fpbid = d.fpbid";


$db->executar($sql);
$db->commit();

$sql = "update seguranca.perfilusuario set pflcod=1082 where usucpf in(

	select foo.iuscpf from (
	select i.iusd, i.iuscpf, case when t.pflcod=1081 and d.doeid is null then 1 else 0 end as ss from sismedio.identificacaousuario i
	inner join sismedio.tipoperfil t on t.iusd = i.iusd
	left join sismedio.definicaoorientadoresestudo d on d.iusd=i.iusd AND d.doecodigoinep=i.iuscodigoinep
	where i.iusstatus='A'
	) foo where foo.ss=1

	) and pflcod=1081";

$db->executar($sql);
$db->commit();


$sql = "update sismedio.tipoperfil set pflcod=1082 where iusd in(

	select iusd from (
	select i.iusd, i.iuscpf, case when t.pflcod=1081 and d.doeid is null then 1 else 0 end as ss from sismedio.identificacaousuario i
	inner join sismedio.tipoperfil t on t.iusd = i.iusd
	left join sismedio.definicaoorientadoresestudo d on d.iusd=i.iusd AND d.doecodigoinep=i.iuscodigoinep
	where i.iusstatus='A'
	) foo where foo.ss=1

	) and pflcod=1081";

$db->executar($sql);
$db->commit();

	
$sql = "update sismedio.identificacaousuario x set uncid=null where iusd in(

select i.iusd from sismedio.identificacaousuario i 
inner join sismedio.tipoperfil t on t.iusd = i.iusd and t.pflcod in(1081, 1082, 1088) 
left join sismedio.abrangencia a on a.lemcodigoinep::bigint = i.iuscodigoinep 
where a.abrid is null and i.uncid is not null

)";

$db->executar($sql);
$db->commit();


$sql = "
update sismedio.turmas set uncid=null where iusd in(

select i.iusd from sismedio.identificacaousuario i 
inner join sismedio.tipoperfil t on t.iusd = i.iusd and t.pflcod in(1081, 1082, 1088)
inner join sismedio.turmas tu on tu.iusd = i.iusd 
left join sismedio.abrangencia a on a.lemcodigoinep::bigint = i.iuscodigoinep 
where a.abrid is null and tu.uncid is not null


)";

$db->executar($sql);
$db->commit();

	
$sql = "update sismedio.mensarioavaliacoes x set mavmonitoramento=foo.fatmonitoramento from (

			select mm.*, f.*, d.esdid from sismedio.mensario m
			inner join sismedio.tipoperfil t on t.iusd = m.iusd and t.pflcod!=1082 
			LEFT JOIN sismedio.pagamentobolsista pg ON pg.iusd = t.iusd AND m.fpbid = pg.fpbid 
			INNER JOIN sismedio.fatoresdeavaliacao f ON f.fatpflcodavaliado = CASE WHEN pg.pflcod IS NOT NULL THEN pg.pflcod ELSE t.pflcod END
			inner join workflow.documento d on d.docid = m.docid and d.esdid in(951,957)
			inner join sismedio.mensarioavaliacoes mm on mm.menid = m.menid
			where mavmonitoramento=0
		
			) foo where foo.mavid = x.mavid";

$db->executar($sql);
$db->commit();

$sql = "update sismedio.mensarioavaliacoes ma SET mavtotal=foo.total from (
			select * from (
			select
			mavid,
			mavfrequencia,
			mavatividadesrealizadas,
			mavmonitoramento,
			mavtotal,
			(coalesce((mavfrequencia*fatfrequencia),0) + coalesce((mavatividadesrealizadas*fatatividadesrealizadas),0) + coalesce(mavmonitoramento,0)) as total
			from sismedio.mensarioavaliacoes ma
			inner join sismedio.mensario m ON m.menid = ma.menid
			inner join sismedio.identificacaousuario u ON u.iusd = m.iusd
			inner join sismedio.tipoperfil t ON t.iusd = u.iusd 
			LEFT JOIN sismedio.pagamentobolsista pg ON pg.iusd = t.iusd AND m.fpbid = pg.fpbid 
			inner join sismedio.fatoresdeavaliacao f ON f.fatpflcodavaliado = CASE WHEN pg.pflcod IS NOT NULL THEN pg.pflcod ELSE t.pflcod END
			) fee
			where fee.mavtotal != total
			) foo
			where ma.mavid = foo.mavid";

$db->executar($sql);
$db->commit();

$sql = "select i.iusd, i.iusnome, fu.fpbid, m.menid, ma.mavid from sismedio.identificacaousuario i
inner join sismedio.tipoperfil t on t.iusd = i.iusd
inner join sismedio.universidadecadastro u on u.uncid = i.uncid
inner join workflow.documento d on d.docid = u.docid
inner join workflow.documento d2 on d2.docid = u.docidturmaformadoresregionais
inner join workflow.documento d3 on d3.docid = u.docidturmaorientadoresestudo
inner join sismedio.folhapagamentouniversidade fu on fu.uncid = i.uncid and fu.pflcod = t.pflcod
inner join sismedio.folhapagamento f on f.fpbid = fu.fpbid
left join sismedio.mensario m on m.iusd = i.iusd and m.fpbid = fu.fpbid
left join sismedio.mensarioavaliacoes ma on ma.menid = m.menid
where i.iusstatus='A' and ma.mavid is null and t.pflcod=1076 and d.esdid=931 and d2.esdid=1200 and d3.esdid=1200 and to_char(NOW(),'YYYYmmdd')>=to_char((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'15')::date,'YYYYmmdd')
";
	
$arr = $db->carregar($sql);

if($arr[0]) {
	foreach($arr as $ar) {

		$res = criarMensario(array("iusd" => $ar['iusd'], "fpbid" => $ar['fpbid']));

		if($res['esdid']==ESD_ENVIADO_MENSARIO || $res['esdid']==ESD_APROVADO_MENSARIO) {
			$mavmonitoramento = '5';
			$mavtotal         = '10.00';
		} else {
			$mavmonitoramento = '0';
			$mavtotal         = '5.00';
		}

		$sql = "INSERT INTO sismedio.mensarioavaliacoes(
			iusdavaliador, menid, mavatividadesrealizadas, mavmonitoramento, mavtotal)
			VALUES ('".IUS_AVALIADOR_MEC."', '".$res['memid']."', '1.0', '{$mavmonitoramento}', '{$mavtotal}');";

		$db->executar($sql);

	}

	$db->commit();
}
	
	
	
	
$sql = "update sismedio.identificacaousuario set iusstatus='I' where iusd in(
		select i.iusd from sismedio.identificacaousuario i
		inner join sismedio.tipoperfil t on t.iusd = i.iusd
		inner join sismedio.listaescolasensinomedio l on l.lemcodigoinep::bigint = i.iuscodigoinep
		inner join workflow.documento d on d.docid = l.docid
		where d.esdid=1151 and t.pflcod in(1082,1088,1081)
		)";

$db->executar($sql);
$db->commit();
	
	
$sql = "update sismedio.identificacaousuario x set uncid=foo.uncid from (

		select i.iusd, u.uncid
		from sismedio.identificacaousuario i
		inner join sismedio.tipoperfil t on t.iusd = i.iusd and t.pflcod in(1081, 1082, 1088)
		inner join sismedio.abrangencia a on a.lemcodigoinep::bigint = i.iuscodigoinep
		inner join sismedio.estruturacurso e on e.ecuid = a.ecuid
		inner join sismedio.universidadecadastro u on u.uncid = e.uncid
		inner join workflow.documento d on d.docid = u.docid
		where d.esdid=931
	
		) foo where foo.iusd = x.iusd";

$db->executar($sql);
$db->commit();
	
	
$sql = "update sismedio.identificacaousuario x set uncid=foo.uncid from (
select u.uncid, a.lemcodigoinep::bigint from sismedio.universidadecadastro u
inner join sismedio.estruturacurso e on e.uncid = u.uncid
inner join sismedio.abrangencia a on a.ecuid = e.ecuid
inner join workflow.documento d1 on d1.docid = u.docidturmaformadoresregionais
inner join workflow.documento d2 on d2.docid = u.docidturmaorientadoresestudo
where d1.esdid=1200 and d2.esdid=1200
) foo where x.iuscodigoinep=foo.lemcodigoinep and x.uncid is null";

$db->executar($sql);
$db->commit();
	
$sql = "update sismedio.identificacaousuario x set muncodatuacao=foo.muncod from (
select i.iusd, l.muncod from sismedio.identificacaousuario i
inner join sismedio.tipoperfil t on t.iusd = i.iusd and t.pflcod in(1082,1081,1088)
inner join sismedio.listaescolasensinomedio l on l.lemcodigoinep::bigint = i.iuscodigoinep
where iusstatus='A') foo where x.iusd = foo.iusd and x.muncodatuacao is null";

$db->executar($sql);
$db->commit();
	
$sql = "update sismedio.identificacaousuario x set muncodatuacao=foo.muncod from (
select i.iusd, uu.muncod from sismedio.identificacaousuario i
inner join sismedio.tipoperfil t on t.iusd = i.iusd and t.pflcod in(1076,1079,1190,1078,1077)
inner join sismedio.universidadecadastro u on u.uncid = i.uncid
inner join sismedio.universidade uu on uu.uniid = u.uniid
where iusstatus='A') foo where x.iusd = foo.iusd and x.muncodatuacao is null";

$db->executar($sql);
$db->commit();

$sql = "update sismedio.identificacaousuario x set muncodatuacao=foo.muncod from (
select i.iusd, l.muncod from sismedio.identificacaousuario i 
inner join sismedio.tipoperfil t on t.iusd = i.iusd 
inner join sismedio.listaescolasensinomedio l on l.lemcodigoinep::bigint = i.iuscodigoinep
where t.pflcod in(1082,1088) and i.muncodatuacao is null
) foo where x.iusd = foo.iusd";

$db->executar($sql);
$db->commit();
	
$sql = "update sismedio.tipoperfil x set pflcod=1081 from (

		select t.tpeid, i.iusd from sismedio.identificacaousuario i
		inner join sismedio.tipoperfil t on t.iusd = i.iusd
		inner join sismedio.listaescolasensinomedio l on l.lemcodigoinep::bigint = i.iuscodigoinep
		inner join workflow.documento d on d.docid = l.docid
		inner join sismedio.definicaoorientadoresestudo oi on oi.iusd = i.iusd
		inner join sismedio.universidadecadastro u on u.uncid = i.uncid
		inner join workflow.documento d2 on d2.docid = u.docid
		where i.iusstatus='A' and t.pflcod in('1082','1088') and d.esdid=1091 and d2.esdid=931
	
		) foo where foo.tpeid = x.tpeid";

$db->executar($sql);
$db->commit();
	
$sql = "INSERT INTO sismedio.turmas(
            uncid, iusd, turdesc, turstatus, picid, muncod)

			select i.uncid, i.iusd, 'TURMA FR - '||i.iusnome as turma, 'A', null, null from sismedio.identificacaousuario i 
			inner join sismedio.tipoperfil t on t.iusd = i.iusd 
			inner join sismedio.universidadecadastro u on u.uncid = i.uncid 
			inner join workflow.documento d on d.docid = u.docid 
			left join sismedio.turmas tu on tu.iusd = i.iusd 
			where i.iusstatus='A' AND t.pflcod=1190 and d.esdid=931 and tu.turid is null";
	
$db->executar($sql);
$db->commit();
	
$sql = "INSERT INTO sismedio.turmas(
            uncid, iusd, turdesc, turstatus, picid, muncod)

			select i.uncid, i.iusd, 'TURMA '||l.lemnomeescola||' - '||i.iusnome as turma, 'A', null, null from sismedio.identificacaousuario i 
			inner join sismedio.tipoperfil t on t.iusd = i.iusd 
			inner join sismedio.universidadecadastro u on u.uncid = i.uncid 
			inner join workflow.documento d on d.docid = u.docid 
			inner join sismedio.listaescolasensinomedio l on l.lemcodigoinep::bigint = i.iuscodigoinep 
			inner join workflow.documento d2 on d2.docid = l.docid 
			left join sismedio.turmas tu on tu.iusd = i.iusd 
			where i.iusstatus='A' AND t.pflcod=1081 and d.esdid=931 and d2.esdid=1091 and tu.turid is null";
	
$db->executar($sql);
$db->commit();
	
$sql = "INSERT INTO sismedio.orientadorturma(
            turid, iusd, otustatus, otudata)

			select 
			
			tu.turid,i.iusd,'A',now()
			
			from sismedio.identificacaousuario i 
			inner join sismedio.tipoperfil t on t.iusd = i.iusd 
			inner join sismedio.identificacaousuario i2 on i2.iuscodigoinep = i.iuscodigoinep 
			inner join sismedio.tipoperfil t2 on t2.iusd = i2.iusd and t2.pflcod=1081 
			inner join sismedio.turmas tu on tu.iusd = i2.iusd 
			left join sismedio.orientadorturma ot on ot.iusd = i.iusd 
			where t.pflcod in(1082,1088) and ot.otuid is null and i.iuscodigoinep in(
			
			select i2.iuscodigoinep as t from sismedio.identificacaousuario i2 
			inner join sismedio.tipoperfil t2 on i2.iusd = t2.iusd 
			where t2.pflcod=1081 and i2.iusstatus='A' 
			group by i2.iuscodigoinep 
			having count(*)=1
			
			)";
	
$db->executar($sql);
$db->commit();


$sql = "update sismedio.identificacaousuario x set iustipoprofessor='cpflivre' where iusd in(

select i.iusd from sismedio.identificacaousuario i 
inner join sismedio.tipoperfil t on t.iusd = i.iusd 
where i.iusstatus='A' and t.pflcod=1082 and iustipoprofessor is null

)";

$db->executar($sql);
$db->commit();


$sql = "update sismedio.identificacaousuario x set iustipoprofessor=foo.tipo from (

select * from (

select i.iusd, case when p.cpf is null then 'cpflivre' else 'censo' end as tipo, i.iustipoprofessor from sismedio.identificacaousuario i 
inner join sismedio.tipoperfil t on t.iusd = i.iusd 
left join sismedio.professoresalfabetizadores p on p.cpf = i.iuscpf
where i.iusstatus='A' and t.pflcod=1082

) faa where faa.tipo!=faa.iustipoprofessor or faa.iustipoprofessor is null

) foo where x.iusd = foo.iusd";

$db->executar($sql);
$db->commit();

	
	$sql = "INSERT INTO sispacto2.orientadorturma(
            turid, iusd, otustatus, otudata)
select (select turid from sispacto2.turmas tu inner join sispacto2.identificacaousuario i2 on i2.iusd = tu.iusd inner join sispacto2.tipoperfil t2 on t2.iusd = i2.iusd where t2.pflcod=1131 and i2.uncid=i.uncid order by random() limit 1) as turid, i.iusd, 'A', now() from sispacto2.identificacaousuario i 
inner join sispacto2.tipoperfil t on t.iusd = i.iusd 
inner join sispacto2.universidadecadastro u on u.uncid = i.uncid 
inner join workflow.documento d on d.docid = u.docid 
inner join workflow.documento d2 on d2.docid = u.docidformacaoinicial
left join sispacto2.orientadorturma ot on ot.iusd = i.iusd 
where iusstatus='A' and t.pflcod=1120 and d.esdid=993 and d2.esdid=1010 and ot.otuid is null";
	
	$db->executar($sql);
	$db->commit();
	
	$sql = "UPDATE sismedio.identificacaousuario SET iusemailprincipal=replace(iusemailprincipal,'@com','@meudominio.com') WHERE iusemailprincipal ilike '%@com%';";
	$db->executar($sql);
	$db->commit();
	
	$sql = "UPDATE sismedio.identificacaousuario SET iusemailprincipal=replace(iusemailprincipal,'@.','@') WHERE iusemailprincipal ILIKE '%@.%';";
	$db->executar($sql);
	$db->commit();
	
	$sql = "select distinct i.iusd, s.logresponse from sismedio.identificacaousuario i 
	inner join sismedio.tipoperfil t on t.iusd = i.iusd 
	inner join log_historico.logsgb_sismedio s on s.logcpf = i.iuscpf and s.logservico='gravarDadosBolsista' and s.logerro=true
	where cadastradosgb=false and iustermocompromisso=true and logresponse ilike '%Erro: 00026:%';";
	
	$arr = $db->carregar($sql);
	
	if($arr[0]) {
		foreach($arr as $ar) {
			$sl = explode("(",$ar['logresponse']);
			$sl = explode(")",$sl[1]);
			
			$iusnome_antigo = $db->pegaUm("SELECT iusnome FROM sismedio.identificacaousuario WHERE iusd='".$ar['iusd']."'");
			
			// somente atualizar se os 9 primeiros digitos forem semelhantes
			if(substr(strtoupper($iusnome_antigo),0,9)==substr(strtoupper(trim($sl[0])),0,9)) {
				$sql = "UPDATE sismedio.identificacaousuario SET iusnome='".trim($sl[0])."' WHERE iusd='".$ar['iusd']."'";
				$db->executar($sql);
			}
		}
		$db->commit();
	}

}

// black list
$pularcpf = $db->carregarColuna("SELECT lnscpf FROM sismedio.listanegrasgb");


$sql = "SELECT DISTINCT i.iusd, l.logcpf FROM sismedio.identificacaousuario i 
		LEFT JOIN log_historico.logsgb_sismedio l ON l.logcpf = i.iuscpf
		WHERE iustermocompromisso=true AND cadastradosgb=false".(($pularcpf)?" AND iuscpf NOT IN('".implode("','",$pularcpf)."')":"")." ORDER BY l.logcpf DESC";
$iusds = $db->carregarColuna($sql);

libxml_use_internal_errors( true );

if($iusds) {
	foreach($iusds as $iusd) {
		
		$lnsid = $db->pegaUm("INSERT INTO sismedio.listanegrasgb(lnscpf) VALUES ((SELECT iuscpf FROM sismedio.identificacaousuario WHERE iusd='".$iusd."')) RETURNING lnsid;");
		$db->commit();
		
		sincronizarDadosUsuarioSGB(array("iusd" => $iusd, "sincronizacao" => true));
		
		$db->executar("DELETE FROM sismedio.listanegrasgb WHERE lnsid='".$lnsid."'");
		$db->commit();
		
	}
}


echo "Sincronizar USUARIOS DO SISMEDIO NO SGB - OK";


$sql = "SELECT uncid FROM sismedio.universidadecadastro WHERE (cadastrosgb=false OR cadastrosgb IS NULL)";
$uncids = $db->carregarColuna($sql);

libxml_use_internal_errors( true );

if($uncids) {
	foreach($uncids as $uncid) {
		sincronizarDadosEntidadeSGB(array("uncid" => $uncid));
	}
}


echo "Sincronizar ENTIDADES DO SISMEDIO NO SGB - OK";

$sql = "UPDATE seguranca.agendamentoscripts SET agstempoexecucao='".round((getmicrotime() - $microtime),2)."' WHERE agsfile='sismedio_sincronizar_usuarios_sgb.php'";
$db->executar($sql);
$db->commit();


echo "inserindo certificação";

$sql = "DELETE FROM sismedio.certificacao";
$db->executar($sql);
$db->commit();

$sql = "INSERT INTO sismedio.certificacao(
iusd, pflcod, cerfrequencia)
select foo.iusd, foo.pflcod, case when foo.freq > 100 then 100.0 else foo.freq end as cerfrequencia from (
select i.iusd, case when (select count(*) from sismedio.folhapagamentouniversidade fp inner join sismedio.folhapagamento f on f.fpbid=fp.fpbid and fp.rfustatus='A' where pflcod=m.pflcod and uncid=i.uncid) > 0 then round((sum(ma.mavfrequencia)/(select count(*) from sismedio.folhapagamentouniversidade fp inner join sismedio.folhapagamento f on f.fpbid=fp.fpbid and fp.rfustatus='A' where pflcod=m.pflcod and uncid=i.uncid))*100,1) else 0 end as freq, m.pflcod from sismedio.mensario m
inner join sismedio.mensarioavaliacoes ma on ma.menid = m.menid
inner join sismedio.identificacaousuario i on i.iusd = m.iusd
where m.pflcod=1081 and i.uncid is not null
group by i.iusd,m.pflcod,i.uncid
) foo";

$db->executar($sql);
$db->commit();

$sql = "INSERT INTO sismedio.certificacao(
iusd, pflcod, cerfrequencia)
select i.iusd, t.pflcod, '0.0' as freq from sismedio.identificacaousuario i
inner join sismedio.tipoperfil t on t.iusd = i.iusd and t.pflcod=1081
left join sismedio.certificacao c on c.iusd = i.iusd
where c.cerfrequencia is null and i.uncid is not null and i.iuscpf not ilike 'SIS%'";

$db->executar($sql);
$db->commit();

$sql = "INSERT INTO sismedio.certificacao(
iusd, pflcod, cerfrequencia)
select foo.iusd, foo.pflcod, case when foo.freq > 100 then 100.0 else foo.freq end as cerfrequencia from (
select i.iusd, case when (select count(*) from sismedio.folhapagamentouniversidade fp inner join sismedio.folhapagamento f on f.fpbid=fp.fpbid and fp.rfustatus='A' where pflcod=m.pflcod and uncid=i.uncid) > 0 then round((sum(ma.mavfrequencia)/(select count(*) from sismedio.folhapagamentouniversidade fp inner join sismedio.folhapagamento f on f.fpbid=fp.fpbid and fp.rfustatus='A' where pflcod=m.pflcod and uncid=i.uncid))*100,1) else 0 end as freq, m.pflcod from sismedio.mensario m 
inner join sismedio.mensarioavaliacoes ma on ma.menid = m.menid
inner join sismedio.identificacaousuario i on i.iusd = m.iusd
where m.pflcod in(1082,1088) and i.uncid is not null
group by i.iusd,m.pflcod,i.uncid
) foo";

$db->executar($sql);
$db->commit();

$sql = "INSERT INTO sismedio.certificacao(
iusd, pflcod, cerfrequencia)
select i.iusd, t.pflcod, 0.0 as freq from sismedio.identificacaousuario i
inner join sismedio.tipoperfil t on t.iusd = i.iusd and t.pflcod in(1082,1088)
left join sismedio.certificacao c on c.iusd = i.iusd
where i.iusd is null and i.uncid is not null and i.iuscpf not ilike 'SIS%'";

$db->executar($sql);
$db->commit();

$sql = "UPDATE sismedio.certificacao SET cerfrequencia=0.0 WHERE cerfrequencia IS NULL";

$db->executar($sql);
$db->commit();

/*
 * ENVIANDO EMAIL CONFIRMANDO O PROCESSAMENTO
 */
require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= "SISMédio - Sincronizar Usuários SGB";
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

$db->close();

?>