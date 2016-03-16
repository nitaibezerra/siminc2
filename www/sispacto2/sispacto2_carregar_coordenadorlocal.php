<?php

function getmicrotime()
{list($usec, $sec) = explode(" ", microtime());
 return ((float)$usec + (float)$sec);} 

date_default_timezone_set ('America/Sao_Paulo');

$_REQUEST['baselogin'] = "simec_espelho_producao";

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(0);
/* FIM configurações */


// carrega as funções gerais
//include_once "/var/www/simec/global/config.inc";
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

error_reporting(1);

include "_constantes.php";

// CPF do administrador de sistemas
if(!$_SESSION['usucpf'])
$_SESSION['usucpforigem'] = '';

$_SESSION['usucpf'] = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

//die(md5_encrypt_senha( 'simecdti', '' ));

$sql = "select
mun.estuf,
mun.muncod,
mun.mundescricao,
adp.adprespostapacto as adesao_pacto,
to_char(adp.adpdatarespostapacto, 'dd/mm/yyyy') as data_pacto,
adp.adpresposta as adesao,
to_char(adp.adpdataresposta, 'dd/mm/yyyy') as data_adesao,
pcu.pcunome as coordenador,
pcu.pcucpf as cpf_coordenador,
pcu.pcuemail as email_coordenador,
'(' || pcu.pcudddnumtelefone || ') ' || pcu.pcunumtelefone as telefone_coordenador,
tfo.tfoid as formacaoid,
tfo.tfodsc as formacao,
tvp.tvpid as vinculoid,
tvp.tvpdsc as vinculo,
pff.pffid as funcao_id,
pff.pffdescricao as funcao_atual_similar,
esd.esddsc as situacao,
esd.esdid
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
esd.esdid in( 484, 485, 489 ) 
and mun.muncod not in (select distinct muncod from sispacto2.pactoidadecerta where muncod is not null)
order by
mun.estuf, mun.mundescricao";

$users = $db->carregar($sql);

echo "<pre>";
print_r($users);

if($users[0]) :

foreach($users as $us) {
	
	$existe = $db->pegaUm("select * from sispacto2.pactoidadecerta where muncod='".$us['muncod']."'");
	
	if($existe) {
		
		$erro[] = "Duplicidade nos cadastros - ".$us['cpf_coordenador'];
		
	} else {
		
		$sql = "INSERT INTO sispacto2.pactoidadecerta(
            	estuf, muncod, docid, picselecaopublica, picstatus)
    			VALUES (NULL, '".$us['muncod']."', NULL, FALSE, 'A') returning picid;";
		
    	$picid = $db->pegaUm($sql);
    	
    	$sql = "SELECT iusd FROM sispacto2.identificacaousuario WHERE iuscpf='".$us['cpf_coordenador']."'";
    	$iusd = $db->pegaUm($sql);
    	
    	$emails = explode(";",$us['email_coordenador']);
    	$emails = explode("/",$emails[0]);
    	$emails = explode(" ",$emails[0]);
    	    	
    	if(!$iusd) {
    		
	    	$sql = "INSERT INTO sispacto2.identificacaousuario(
		            picid, muncod, iuscpf, iusnome, iusemailprincipal,  
		            iusdatainclusao, iusstatus, tvpid, funid, foeid)
				    VALUES ('".$picid."', '".$us['muncod']."', '".$us['cpf_coordenador']."', '".$us['coordenador']."', '".$emails[0]."',  
				            NOW(), 'A', '".$us['vinculoid']."', '".$us['funcao_id']."', '".$us['formacaoid']."') returning iusd;";
	    	$iusd = $db->pegaUm($sql);
    	}
    	
    	$tel = explode(" ",$us['telefone_coordenador']);
    	
    	$existe_us = $db->pegaUm("select * from seguranca.usuario where usucpf='".$us['cpf_coordenador']."'");
    	
    	if(!$existe_us) {
    	
	    	$sql = "INSERT INTO seguranca.usuario(
	            	usucpf, usunome, usuemail, usustatus, usufoneddd, usufonenum, ususenha, suscod)
	    			VALUES ('".$us['cpf_coordenador']."', '".substr($us['coordenador'],0,50)."', '".$emails[0]."', 'A', '".trim(str_replace(array("(",")"),array("",""),$tel[0]))."', '".trim($tel[1])."', '".md5_encrypt_senha( 'simecdti', '' )."', 'A');";
	    	$db->executar($sql);
    	
    	}
    	
    	$existe_sis = $db->pegaUm("select * from seguranca.usuario_sistema where usucpf='".$us['cpf_coordenador']."' and sisid='".SIS_SISPACTO."'");
    	
    	if(!$existe_sis) {
    		
	    	$sql = "INSERT INTO seguranca.usuario_sistema(
	        	    usucpf, sisid, susstatus, pflcod, susdataultacesso, suscod)
	    			VALUES ('".$us['cpf_coordenador']."', ".SIS_SISPACTO.", 'A', NULL, NOW(), 'A');";
	    	
	    	$db->executar($sql);
	    	
    	}
    	
    	$existe_tipoperfil = $db->pegaUm("SELECT * FROM sispacto2.tipoperfil WHERE iusd='".$iusd."'");
    	
    	if(!$existe_tipoperfil) {    	
			$sql = "INSERT INTO sispacto2.tipoperfil(
				            iusd, pflcod, tpestatus)
				    	VALUES ('".$iusd."', '".PFL_COORDENADORLOCAL."', 'A');";
	    	$db->executar($sql);
    	}
    	
    	
    	$existe_pfl = $db->pegaUm("select * from seguranca.perfilusuario where usucpf='".$us['cpf_coordenador']."' and pflcod='".PFL_COORDENADORLOCAL."'");
    	
    	if(!$existe_pfl) {
    		$sql = "INSERT INTO seguranca.perfilusuario(usucpf, pflcod) VALUES ('".$us['cpf_coordenador']."', '".PFL_COORDENADORLOCAL."');";
    		$db->executar($sql);
    	}
    	
    	$existe_ur = $db->pegaUm("SELECT * from sispacto2.usuarioresponsabilidade WHERE pflcod='".PFL_COORDENADORLOCAL."' and usucpf='".$us['cpf_coordenador']."'");
    	
    	if(!$existe_ur) {
	    	$sql = "INSERT INTO sispacto2.usuarioresponsabilidade(
	            		pflcod, usucpf, rpustatus, rpudata_inc, muncod)
					    VALUES ('".PFL_COORDENADORLOCAL."', '".$us['cpf_coordenador']."', 'A', NOW(), '".$us['muncod']."');";
	    	$db->executar($sql);
    	}
    	
		
	}
	
}

endif;




if($erro) {
	$db->commit();
	echo "<pre>";
	print_r($erro);
} else {
	$db->commit();
	echo "Ok";
}
?>