<?php
//$_REQUEST['baselogin'] = "simec_espelho_producao"; 

function getmicrotime()
{list($usec, $sec) = explode(" ", microtime());
 return ((float)$usec + (float)$sec);} 

date_default_timezone_set ('America/Sao_Paulo');


// configurações
ini_set("memory_limit", "3000M");
set_time_limit(30000);



// carrega as funções gerais
//include_once "config.inc";
include_once "/var/www/simec/global/config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();


$sql = " delete from mapa.temadado where tmaid in ( 392, 393, 394, 395, 396, 553, 554 );

delete from mapa.valorindicador where dtiid in ( 217 , 218, 220, 221, 222, 223, 224 );

INSERT INTO mapa.valorindicador ( muncod, vliqtd, dtiid ) 
select    

                mun.muncod,
                1, 224
from 

                territorios.municipio mun           

left join 

                par.instrumentounidade inu on inu.muncod = mun.muncod    

left join 

                par.pfadesaoprograma adp on adp.inuid = inu.inuid and adp.tapid in (13,14)

left join 

                workflow.documento doc on doc.docid = adp.docid

left join 

                workflow.estadodocumento esd on esd.esdid = doc.esdid

left join 

                par.pftermoadesaoprograma tap on adp.tapid = tap.tapid and tap.prgid in (157)

left join 

                par.pfcurso pfc on pfc.prgid = tap.prgid and pfcstatus = 'A'

left join 

                par.pfcursista pcu on pcu.adpid = adp.adpid and pcu.pfcid = pfc.pfcid

left join 

                public.tipoformacao tfo on tfo.tfoid = pcu.tfoid

left join 

                public.tipovinculoprofissional tvp on tvp.tvpid = pcu.tvpid

left join 

                par.pffuncao pff on pff.pffid = pcu.pffid           

where 

                adp.adprespostapacto = 'S' 

and

                adp.adpresposta = 'S'

and

                pcu.pcucpf is not null

and 

                esd.esdid = 484

and 

                mun.muncod is not null

order by 

                mun.estuf, mun.mundescricao;



INSERT INTO mapa.valorindicador ( muncod, vliqtd, dtiid ) 
select    

                mun.muncod,
                1, 223
from 

                territorios.municipio mun           

left join 

                par.instrumentounidade inu on inu.muncod = mun.muncod    

left join 

                par.pfadesaoprograma adp on adp.inuid = inu.inuid and adp.tapid in (13,14)

left join 

                workflow.documento doc on doc.docid = adp.docid

left join 

                workflow.estadodocumento esd on esd.esdid = doc.esdid

left join 

                par.pftermoadesaoprograma tap on adp.tapid = tap.tapid and tap.prgid in (157)

left join 

                par.pfcurso pfc on pfc.prgid = tap.prgid and pfcstatus = 'A'

left join 

                par.pfcursista pcu on pcu.adpid = adp.adpid and pcu.pfcid = pfc.pfcid

left join 

                public.tipoformacao tfo on tfo.tfoid = pcu.tfoid

left join 

                public.tipovinculoprofissional tvp on tvp.tvpid = pcu.tvpid

left join 

                par.pffuncao pff on pff.pffid = pcu.pffid           

where 

                adp.adprespostapacto = 'S' 

and

                adp.adpresposta = 'S'

and

                pcu.pcucpf is not null

and 

                esd.esdid = 483

and 

                mun.muncod is not null

order by 

                mun.estuf, mun.mundescricao;



INSERT INTO mapa.valorindicador ( muncod, vliqtd, dtiid ) 
select    

                mun.muncod,

                1, 222

from 

                territorios.municipio mun           

left join 

                par.instrumentounidade inu on inu.muncod = mun.muncod    

left join 

                par.pfadesaoprograma adp on adp.inuid = inu.inuid and adp.tapid in (13,14)

left join 

                workflow.documento doc on doc.docid = adp.docid

left join 

                workflow.estadodocumento esd on esd.esdid = doc.esdid

left join 

                par.pftermoadesaoprograma tap on adp.tapid = tap.tapid and tap.prgid in (157)

left join 

                par.pfcurso pfc on pfc.prgid = tap.prgid and pfcstatus = 'A'

left join 

                par.pfcursista pcu on pcu.adpid = adp.adpid and pcu.pfcid = pfc.pfcid

left join 

                public.tipoformacao tfo on tfo.tfoid = pcu.tfoid

left join 

                public.tipovinculoprofissional tvp on tvp.tvpid = pcu.tvpid

left join 

                par.pffuncao pff on pff.pffid = pcu.pffid           

where 

                adp.adprespostapacto = 'S' 

and

                adp.adpresposta = 'S'

and

                pcu.pcucpf is null

and 

                mun.muncod is not null

order by 

                mun.estuf, mun.mundescricao;




INSERT INTO mapa.valorindicador ( muncod, vliqtd, dtiid ) 
select    

		mun.muncod,

                1, 221

from 

                territorios.municipio mun           

left join 

                par.instrumentounidade inu on inu.muncod = mun.muncod    

left join 

                par.pfadesaoprograma adp on adp.inuid = inu.inuid and adp.tapid in (13,14)

left join 

                workflow.documento doc on doc.docid = adp.docid

left join 

                workflow.estadodocumento esd on esd.esdid = doc.esdid

left join 

                par.pftermoadesaoprograma tap on adp.tapid = tap.tapid and tap.prgid in (157)

left join 

                par.pfcurso pfc on pfc.prgid = tap.prgid and pfcstatus = 'A'

left join 

                par.pfcursista pcu on pcu.adpid = adp.adpid and pcu.pfcid = pfc.pfcid

left join 

                public.tipoformacao tfo on tfo.tfoid = pcu.tfoid

left join 

                public.tipovinculoprofissional tvp on tvp.tvpid = pcu.tvpid

left join 

                par.pffuncao pff on pff.pffid = pcu.pffid           

where 

                adp.adprespostapacto = 'S' 

and        

                (adp.adpresposta is null )

and 

                mun.muncod is not null

order by 

                mun.estuf, mun.mundescricao;





INSERT INTO mapa.valorindicador ( muncod, vliqtd, dtiid ) 
select    

                mun.muncod,

		1, 220

from 

                territorios.municipio mun           

left join 

                par.instrumentounidade inu on inu.muncod = mun.muncod    

left join 

                par.pfadesaoprograma adp on adp.inuid = inu.inuid and adp.tapid in (13,14)

left join 

                workflow.documento doc on doc.docid = adp.docid

left join 

                workflow.estadodocumento esd on esd.esdid = doc.esdid

left join 

                par.pftermoadesaoprograma tap on adp.tapid = tap.tapid and tap.prgid in (157)

left join 

                par.pfcurso pfc on pfc.prgid = tap.prgid and pfcstatus = 'A'

left join 

                par.pfcursista pcu on pcu.adpid = adp.adpid and pcu.pfcid = pfc.pfcid

left join 

                public.tipoformacao tfo on tfo.tfoid = pcu.tfoid

left join 

                public.tipovinculoprofissional tvp on tvp.tvpid = pcu.tvpid

left join 

                par.pffuncao pff on pff.pffid = pcu.pffid           

where 

                adp.adprespostapacto = 'S' 

and

                (adp.adpresposta  = 'N')

and 

                mun.muncod is not null

order by 

                mun.estuf, mun.mundescricao;





INSERT INTO mapa.valorindicador ( muncod, vliqtd, dtiid ) 
select    

                mun.muncod,

                1, 218
from 

                territorios.municipio mun           

left join 

                par.instrumentounidade inu on inu.muncod = mun.muncod    

left join 

                par.pfadesaoprograma adp on adp.inuid = inu.inuid and adp.tapid in (13,14)

left join 

                workflow.documento doc on doc.docid = adp.docid

left join 

                workflow.estadodocumento esd on esd.esdid = doc.esdid

left join 

                par.pftermoadesaoprograma tap on adp.tapid = tap.tapid and tap.prgid in (157)

left join 

                par.pfcurso pfc on pfc.prgid = tap.prgid and pfcstatus = 'A'

left join 

                par.pfcursista pcu on pcu.adpid = adp.adpid and pcu.pfcid = pfc.pfcid

left join 

                public.tipoformacao tfo on tfo.tfoid = pcu.tfoid

left join 

                public.tipovinculoprofissional tvp on tvp.tvpid = pcu.tvpid

left join 

                par.pffuncao pff on pff.pffid = pcu.pffid           

where 

                adp.adprespostapacto = 'N' 

and

                (adp.adpresposta is null or adp.adpresposta = 'N')

and 

                mun.muncod is not null

order by 

                mun.estuf, mun.mundescricao;




INSERT INTO mapa.valorindicador ( muncod, vliqtd, dtiid ) 
select    

                mun.muncod,

                1,
		
		217

from 

                territorios.municipio mun           

left join 

                par.instrumentounidade inu on inu.muncod = mun.muncod    

left join 

                par.pfadesaoprograma adp on adp.inuid = inu.inuid and adp.tapid in (13,14)

left join 

                workflow.documento doc on doc.docid = adp.docid

left join 

                workflow.estadodocumento esd on esd.esdid = doc.esdid

left join 

                par.pftermoadesaoprograma tap on adp.tapid = tap.tapid and tap.prgid in (157)

left join 

                par.pfcurso pfc on pfc.prgid = tap.prgid and pfcstatus = 'A'

left join 

                par.pfcursista pcu on pcu.adpid = adp.adpid and pcu.pfcid = pfc.pfcid

left join 

                public.tipoformacao tfo on tfo.tfoid = pcu.tfoid

left join 

                public.tipovinculoprofissional tvp on tvp.tvpid = pcu.tvpid

left join 

                par.pffuncao pff on pff.pffid = pcu.pffid           

where 

                adp.adprespostapacto is null

order by 

                mun.estuf, mun.mundescricao;


insert into mapa.temadado ( tmaid, tmdvalor, muncod ) 	
select 392, vliqtd, muncod from mapa.valorindicador where dtiid = 217;

insert into mapa.temadado ( tmaid, tmdvalor, muncod ) 	
select 393, vliqtd, muncod from mapa.valorindicador where dtiid = 218;

insert into mapa.temadado ( tmaid, tmdvalor, muncod ) 	
select 394, vliqtd, muncod from mapa.valorindicador where dtiid = 220;

insert into mapa.temadado ( tmaid, tmdvalor, muncod ) 	
select 395, vliqtd, muncod from mapa.valorindicador where dtiid = 221;

insert into mapa.temadado ( tmaid, tmdvalor, muncod ) 	
select 396, vliqtd, muncod from mapa.valorindicador where dtiid = 222;

insert into mapa.temadado ( tmaid, tmdvalor, muncod ) 	
select 553, vliqtd, muncod from mapa.valorindicador where dtiid = 223;

insert into mapa.temadado ( tmaid, tmdvalor, muncod ) 	
select 554, vliqtd, muncod from mapa.valorindicador where dtiid = 224;";

$db->executar($sql);
$db->commit();


require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';

$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= "Atualizando mapa - SISPACTO";
$mensagem->From 		= "noreply@mec.gov.br";
$mensagem->AddAddress($_SESSION['email_sistema'], SIGLA_SISTEMA );
$mensagem->Subject = "Atualizando mapa - SISPACTO";
$mensagem->Body = "Atualizando mapa - SISPACTO<br><br>";
$mensagem->IsHTML( true );
$mensagem->Send();

?>