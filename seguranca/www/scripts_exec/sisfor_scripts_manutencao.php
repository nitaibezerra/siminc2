<?php
header( 'Content-Type: text/html; charset=ISO-8859-1' );
//header( 'Content-Type: text/html; charset=UTF-8' );

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );


error_reporting( E_ALL ^ E_NOTICE );

ini_set("memory_limit", "1024M");
set_time_limit(0);


$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento

// carrega as funчѕes gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
require_once APPRAIZ . "includes/workflow.php";
require_once APPRAIZ . "www/sisfor/_constantes.php";
require_once APPRAIZ . "www/sisfor/_funcoes.php";


// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';

function getmicrotime() {list($usec, $sec) = explode(" ", microtime()); return ((float)$usec + (float)$sec);}

$microtime = getmicrotime();
   
// abre conexчуo com o servidor de banco de dados
$db = new cls_banco();

$sql = "delete from seguranca.perfilusuario p 
using (
SELECT pp.pflcod, i.iuscpf FROM sisfor.identificacaousuario i 
INNER JOIN seguranca.perfilusuario p ON p.usucpf = i.iuscpf 
INNER JOIN seguranca.perfil pp ON pp.pflcod = p.pflcod AND pp.sisid=177
LEFT JOIN sisfor.tipoperfil t ON t.iusd = i.iusd AND t.pflcod = pp.pflcod 
WHERE t.tpeid IS NULL and pp.pflcod IN(1105,1195,1197,1198,1196,1199)
) foo where p.usucpf=foo.iuscpf and p.pflcod=foo.pflcod";

$db->executar($sql);
$db->commit();

// ATUALIZANDO NOME DO CENSO COM NOME VINDO DA RECEITA FEDERAL (SOMENTE SE OS 9 PRIMEIROS DIGITOS FOREM IGUAIS)

$sql = "select distinct i.iusd, s.logresponse from sisfor.identificacaousuario i
		inner join sisfor.tipoperfil t on t.iusd = i.iusd
		inner join sisfor.logsgb s on s.logcpf = i.iuscpf and s.logservico='gravarDadosBolsista' and s.logerro=true
		where cadastradosgb=false and iustermocompromisso=true and logresponse ilike '%Erro: 00026:%'";

$arr = $db->carregar($sql);

if($arr[0]) {
	foreach($arr as $ar) {
		$sl = explode("(",$ar['logresponse']);
		$sl = explode(")",$sl[1]);
			
		$iusnome_antigo = $db->pegaUm("select iusnome from sisfor.identificacaousuario where iusd='".$ar['iusd']."'");
			
		// somente atualizar se os 9 primeiros digitos forem semelhantes
		if(substr(strtoupper($iusnome_antigo),0,9)==substr(strtoupper(trim($sl[0])),0,9)) {
			$sql = "update sisfor.identificacaousuario set iusnome='".trim($sl[0])."' where iusd='".$ar['iusd']."'";
			$db->executar($sql);
		}
	}
	$db->commit();
}

$sql = "update sisfor.identificacaousuario x set muncodatuacao=foo.muncod from (

select e.muncod, i.iusd from sisfor.identificacaousuario i 
inner join sisfor.identificaoendereco e on e.iusd = i.iusd 

) foo where x.iusd = foo.iusd and x.muncodatuacao is null";

$db->executar($sql);
$db->commit();

$sql = "update sisfor.sisfor x set sifprogramasgb=foo2.sifprogramasgb from (

select case when foo.secretaria ilike '%SECADI / %' then 'FCSEC'
	    when foo.secretaria ilike '%SEB / %' then 'FCSEB' end as sifprogramasgb,
       foo.sifid
from (

select 
s.sifid,
case when s.ieoid is not null then cor.coordsigla 
     when s.cnvid is not null then cor2.coordsigla 
     when s.ocuid is not null then cor3.coordsigla 
     when s.oatid is not null then cor4.coordsigla end as secretaria
 

from sisfor.sisfor s 
left join catalogocurso2014.iesofertante ieo on ieo.ieoid = s.ieoid
left join catalogocurso2014.curso cur on cur.curid = ieo.curid
left join catalogocurso2014.coordenacao cor on cor.coordid = cur.coordid
left join sisfor.cursonaovinculado cnv on cnv.cnvid = s.cnvid
left join catalogocurso2014.curso cur2 on cur2.curid = cnv.curid
left join catalogocurso2014.coordenacao cor2 on cor2.coordid = cur2.coordid
left join seguranca.usuario usu on usu.usucpf = s.usucpf
left join sisfor.outrocurso oc on oc.ocuid = s.ocuid
left join catalogocurso2014.coordenacao cor3 on cor3.coordid = oc.coordid 
left join sisfor.outraatividade oat on oat.oatid = s.oatid 
left join catalogocurso2014.coordenacao cor4 on cor4.coordid = oat.coordid
where s.sifprogramasgb is null
) foo where foo.secretaria is not null

) foo2 where foo2.sifid = x.sifid";

$db->executar($sql);
$db->commit();

$sql = "update sisfor.sisfor x set usucpf=foo3.iuscpf from (

select foo2.iuscpf, foo2.sifid from sisfor.sisfor s 
inner join workflow.documento d on d.docid = s.docidprojeto
left join catalogocurso2014.iesofertante ieo on ieo.ieoid = s.ieoid
left join catalogocurso2014.curso cur on cur.curid = ieo.curid
left join catalogocurso2014.coordenacao cor on cor.coordid = cur.coordid
left join sisfor.cursonaovinculado cnv on cnv.cnvid = s.cnvid
left join catalogocurso2014.curso cur2 on cur2.curid = cnv.curid
left join catalogocurso2014.coordenacao cor2 on cor2.coordid = cur2.coordid
left join seguranca.usuario usu on usu.usucpf = s.usucpf
left join sisfor.outrocurso oc on oc.ocuid = s.ocuid
left join catalogocurso2014.coordenacao cor3 on cor3.coordid = oc.coordid 
left join sisfor.outraatividade oat on oat.oatid = s.oatid 
left join catalogocurso2014.coordenacao cor4 on cor4.coordid = oat.coordid

left join (

select i.iuscpf, t.sifid from sisfor.identificacaousuario i 
inner join sisfor.tipoperfil t on t.iusd = i.iusd 
where t.pflcod=1195

) foo on foo.iuscpf=s.usucpf and foo.sifid=s.sifid

left join (

select i.iuscpf, t.sifid from sisfor.identificacaousuario i 
inner join sisfor.tipoperfil t on t.iusd = i.iusd 
where t.pflcod=1195

) foo2 on foo2.sifid=s.sifid

where d.esdid=1187 and sifstatus='A' 
and (cor.coordsigla ilike '%SECADI%' or cor2.coordsigla ilike '%SECADI%' or cor3.coordsigla ilike '%SECADI%' or cor4.coordsigla ilike '%SECADI%') 
and sifexecucaosisfor=true 
and foo.iuscpf is null and foo2.iuscpf is not null
) foo3 where x.sifid=foo3.sifid";

$db->executar($sql);
$db->commit();



$sql = "update sisfor.sisfor x set usucpf=foo3.iuscpf from (

select foo2.iuscpf, foo2.sifid from sisfor.sisfor s 
inner join workflow.documento d on d.docid = s.docidprojeto
left join catalogocurso2014.iesofertante ieo on ieo.ieoid = s.ieoid
left join catalogocurso2014.curso cur on cur.curid = ieo.curid
left join catalogocurso2014.coordenacao cor on cor.coordid = cur.coordid
left join sisfor.cursonaovinculado cnv on cnv.cnvid = s.cnvid
left join catalogocurso2014.curso cur2 on cur2.curid = cnv.curid
left join catalogocurso2014.coordenacao cor2 on cor2.coordid = cur2.coordid
left join seguranca.usuario usu on usu.usucpf = s.usucpf
left join sisfor.outrocurso oc on oc.ocuid = s.ocuid
left join catalogocurso2014.coordenacao cor3 on cor3.coordid = oc.coordid 
left join sisfor.outraatividade oat on oat.oatid = s.oatid 
left join catalogocurso2014.coordenacao cor4 on cor4.coordid = oat.coordid

left join (

select i.iuscpf, t.sifid from sisfor.identificacaousuario i 
inner join sisfor.tipoperfil t on t.iusd = i.iusd 
where t.pflcod=1105

) foo on foo.iuscpf=s.usucpf and foo.sifid=s.sifid

left join (

select i.iuscpf, t.sifid from sisfor.identificacaousuario i 
inner join sisfor.tipoperfil t on t.iusd = i.iusd 
where t.pflcod=1105

) foo2 on foo2.sifid=s.sifid

where d.esdid=1187 and sifstatus='A' 
and (cor.coordsigla ilike '%SEB%' or cor2.coordsigla ilike '%SEB%' or cor3.coordsigla ilike '%SEB%' or cor4.coordsigla ilike '%SEB%') 
and sifexecucaosisfor=true 
and foo.iuscpf is null and foo2.iuscpf is not null
) foo3 where x.sifid=foo3.sifid";

$db->executar($sql);
$db->commit();

$sql = "update sisfor.sisfor x set tpeid=foo.tpeid from (
select s.sifid, t.tpeid from sisfor.sisfor s 
inner join sisfor.identificacaousuario i on i.iuscpf = s.usucpf 
inner join sisfor.tipoperfil t on t.iusd = i.iusd and s.sifid = t.sifid and t.pflcod in(1105,1195)
where s.tpeid is null
) foo where x.sifid=foo.sifid";

$db->executar($sql);
$db->commit();


$sql = "delete from sisfor.folhapagamentoprojeto where sifid in(
select sifid from sisfor.sisfor where sifexecucaosisfor=false
)";

$db->executar($sql);
$db->commit();

$sql = "update sisfor.cursista x set curnome=foo.novo from (
select upper(removeacento(curnome)) as novo, curid from sisfor.cursista where curcpf is null
) foo where x.curid = foo.curid";

$db->executar($sql);
$db->commit();

$sql = "update sisfor.relatoriomensal x set remparcela=foo.parcela from (
select r.iusd, count(*) as parcela, max(remid) as remid from sisfor.relatoriomensal r 
where remparcela is null
group by r.iusd
) foo 
where x.remid = foo.remid";

$db->executar($sql);
$db->commit();



$sql = "UPDATE seguranca.agendamentoscripts SET agstempoexecucao='".round((getmicrotime() - $microtime),2)."' WHERE agsfile='sisfor_scripts_manutencao.php'";
$db->executar($sql);
$db->commit();


$db->close();


?>