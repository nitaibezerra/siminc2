<?php
header( 'Content-Type: text/html; charset=ISO-8859-1' );
//header( 'Content-Type: text/html; charset=UTF-8' );

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );


error_reporting( E_ALL ^ E_NOTICE );

ini_set("memory_limit", "1024M");
set_time_limit(0);


$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
require_once APPRAIZ . "includes/workflow.php";
require_once APPRAIZ . "www/sispacto2/_constantes.php";
require_once APPRAIZ . "www/sispacto2/_funcoes.php";

require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';


// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';

function getmicrotime() {list($usec, $sec) = explode(" ", microtime()); return ((float)$usec + (float)$sec);}

$microtime = getmicrotime();
   
// abre conexção com o servidor de banco de dados
$db = new cls_banco();

// ATUALIZANDO AS CARACTERISTICAS DAS TURMAS NA QUAL O ORIENTADOR DE ESTUDOS VIRA FORMADOR

$sql = "UPDATE sispacto2.turmas x set uncid=foo.uncid, turdesc=foo.turdesc FROM (

select i.uncid, t.turid, replace(turdesc,'Turma OE','Turma FR') as turdesc FROM sispacto2.turmas t
inner join sispacto2.identificacaousuario i on i.iusd = t.iusd
inner join sispacto2.tipoperfil tp on tp.iusd = i.iusd and tp.pflcod=1131
inner join seguranca.perfil pe on pe.pflcod = tp.pflcod
where t.uncid is null

) foo where x.turid=foo.turid";

$db->executar($sql);
$db->commit();


// ATUALIZANDO NOTA FINAL DE BOLSISTAS CONFORME AS AVALIAÇÕES E O PERFIL NO MOMENTO DA AVALIAÇÃO (MUITO COMUM EM BOLSISTAS QUE MUDAM DE PERFIL DURANTE O PROGRAMA)

$fpbids = $db->carregarColuna("SELECT fpbid FROM sispacto2.folhapagamento");

if($fpbids) {
	foreach($fpbids as $fpbid) {

		$sql = "UPDATE sispacto2.mensarioavaliacoes ma SET mavtotal=foo.total FROM (
                                               select
                                               mavid,
                                               mavfrequencia,
                                               mavatividadesrealizadas,
                                               mavmonitoramento,
                                               mavtotal,
                                               (coalesce((mavfrequencia*fatfrequencia),0) + coalesce((mavatividadesrealizadas*fatatividadesrealizadas),0) + coalesce(mavmonitoramento,0)) as total
                                               FROM sispacto2.mensario m
                                               inner join sispacto2.mensarioavaliacoes ma ON m.menid = ma.menid 
					       inner join workflow.documento d ON d.docid = m.docid 
                                               inner join sispacto2.fatoresdeavaliacao f ON f.fatpflcodavaliado = m.pflcod 
					       where d.esdid!=1015 and m.fpbid={$fpbid} and ((coalesce((mavfrequencia*fatfrequencia),0) + coalesce((mavatividadesrealizadas*fatatividadesrealizadas),0) + coalesce(mavmonitoramento,0)))!=mavtotal
                                               ) foo
                                               where ma.mavid = foo.mavid";
		
		$db->executar($sql);
		$db->commit();
		
		
	}

}

// ATUALIZANDO O VINCULO COM A UNIVERSIDADE NO SIMEC COM BASE NA ABRANGENCIA DO MUNICIPIO (CL,OE E PA). MUITO COMUM QUANDO AS UNIVERSIDADES TROCAM MUNICIPIOS DE ABRANGENCIA NO DECORRER DO CURSO

$sql = "UPDATE sispacto2.usuarioresponsabilidade x set uncid=foo.uncidnovo FROM (

select i.iusd, i.iuscpf, e.uncid as uncidnovo FROM sispacto2.identificacaousuario i 
inner join sispacto2.tipoperfil t on t.iusd = i.iusd and t.pflcod in(1119,1120,1118)
inner join sispacto2.pactoidadecerta p on p.picid = i.picid 
inner join sispacto2.abrangencia a on a.muncod = p.muncod and a.esfera='M' 
inner join sispacto2.estruturacurso e on e.ecuid = a.ecuid 
where p.muncod is not null and i.uncid != e.uncid

) foo where x.usucpf = foo.iuscpf";

$db->executar($sql);
$db->commit();




// SCRIPT QUE AVALIA AUTOMATICAMENTE OS CGs CASO ELES JA ESTEJAM COM O PROJETO VALIDADO

$sql = "select i.iusd, i.iusnome, fu.fpbid, m.menid, ma.mavid FROM sispacto2.identificacaousuario i 
inner join sispacto2.tipoperfil t on t.iusd = i.iusd 
inner join sispacto2.universidadecadastro u on u.uncid = i.uncid 
inner join workflow.documento d on d.docid = u.docid 
inner join workflow.documento d2 on d2.docid = u.docidformacaoinicial 
inner join workflow.documento d3 on d3.docid = u.docidturma 
inner join sispacto2.folhapagamentouniversidade fu on fu.uncid = i.uncid and fu.pflcod = t.pflcod 
inner join sispacto2.folhapagamento f on f.fpbid = fu.fpbid 
left join sispacto2.mensario m on m.iusd = i.iusd and m.fpbid = fu.fpbid 
left join sispacto2.mensarioavaliacoes ma on ma.menid = m.menid
where i.iusstatus='A' and ma.mavid is null and t.pflcod=1117 and d.esdid=993 and d2.esdid=1010 and d3.esdid=985 and to_char(NOW(),'YYYYmmdd')>=to_char((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'15')::date,'YYYYmmdd')
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
		
		$sql = "INSERT INTO sispacto2.mensarioavaliacoes(
				iusdavaliador, menid, mavatividadesrealizadas, mavmonitoramento, mavtotal)
				VALUES ('".IUS_AVALIADOR_MEC."', '".$res['memid']."', '1.0', '{$mavmonitoramento}', '{$mavtotal}');";
		
		$db->executar($sql);

	}
	
	$db->commit();
}

// ATUALIZA A TABELA DE USUARIO DO SIMEC COM NOME VALIDADO NA RECEITA FEDERAL

$sql = "UPDATE seguranca.usuario x set usunome=foo.iusnome FROM (
select i.iusnome, u.usucpf FROM sispacto2.identificacaousuario i 
inner join seguranca.usuario u on u.usucpf = i.iuscpf 
where cadastradosgb=true and removeacento(i.iusnome) != removeacento(u.usunome)
) foo where x.usucpf = foo.usucpf";

$db->executar($sql);
$db->commit();



$sql = "UPDATE sispacto2.identificacaousuario x set iusnome=foo.nome FROM (
select iusd, substr(iusnome,0,strpos(iusnome,'<')) as nome FROM sispacto2.identificacaousuario where iusnome ilike '%<%'
) foo where x.iusd = foo.iusd";

$db->executar($sql);
$db->commit();


$sql = "UPDATE sispacto2.usuarioresponsabilidade x SET uncid=xx.uncid FROM (

			select * FROM (
		
			select i.uncid, 
				   (select uncid FROM sispacto2.usuarioresponsabilidade where usucpf=i.iuscpf and pflcod=t.pflcod and uncid is not null and rpustatus='A' limit 1) as uncid2, 
				   i.iuscpf, 
				   t.pflcod 
			FROM sispacto2.identificacaousuario i
			inner join sispacto2.tipoperfil t on t.iusd = i.iusd
		
			) foo where foo.uncid!=foo.uncid2
		
			) xx where x.usucpf=xx.iuscpf and x.pflcod=xx.pflcod and rpustatus='A'";

$db->executar($sql);
$db->commit();


$sql = "UPDATE sispacto2.identificacaousuario set iusemailprincipal=replace(iusemailprincipal,'@com','@meudominio.com') where iusemailprincipal ilike '%@com%';";
$db->executar($sql);
$db->commit();

$sql = "UPDATE sispacto2.identificacaousuario set iusemailprincipal=replace(iusemailprincipal,'@.','@') where iusemailprincipal ilike '%@.%';";
$db->executar($sql);
$db->commit();

// ATUALIZANDO NOME DO CENSO COM NOME VINDO DA RECEITA FEDERAL (SOMENTE SE OS 9 PRIMEIROS DIGITOS FOREM IGUAIS)

$sql = "select distinct i.iusd, s.logresponse FROM sispacto2.identificacaousuario i
		inner join sispacto2.tipoperfil t on t.iusd = i.iusd
		inner join log_historico.logsgb_sispacto2 s on s.logcpf = i.iuscpf and s.logservico='gravarDadosBolsista' and s.logerro=true
		where cadastradosgb=false and iustermocompromisso=true and logresponse ilike '%Erro: 00026:%';";

$arr = $db->carregar($sql);

if($arr[0]) {
	foreach($arr as $ar) {
		$sl = explode("(",$ar['logresponse']);
		$sl = explode(")",$sl[1]);
			
		$iusnome_antigo = $db->pegaUm("select iusnome FROM sispacto2.identificacaousuario where iusd='".$ar['iusd']."'");
			
		// somente atualizar se os 9 primeiros digitos forem semelhantes
		if(substr(strtoupper($iusnome_antigo),0,9)==substr(strtoupper(trim($sl[0])),0,9)) {
			$sql = "UPDATE sispacto2.identificacaousuario set iusnome='".trim($sl[0])."' where iusd='".$ar['iusd']."'";
			$db->executar($sql);
		}
	}
	$db->commit();
}


$sql = "UPDATE sispacto2.identificacaousuario set iustipoorientador='orientadorsispacto2013' where iusd in(

		select i.iusd FROM sispacto2.identificacaousuario i INNER JOIN sispacto2.tipoperfil t on t.iusd = i.iusd where t.pflcod=1120 and iustipoorientador!='orientadorsispacto2013' and i.iuscpf in(
		
		select i.iuscpf FROM sispacto.identificacaousuario i 
		inner join sispacto.tipoperfil t on t.iusd = i.iusd and t.pflcod=827 
		inner join sispacto.mensario m on m.iusd = i.iusd 
		inner join sispacto.mensarioavaliacoes ma on ma.menid = m.menid 
		where mavrecomendadocertificacao=1
		
		)
		
		)";

$db->executar($sql);
$db->commit();

$sql = "UPDATE sispacto2.identificacaousuario set iustipoorientador='professorsispacto2013' where iusd in(

		select i.iusd FROM sispacto2.identificacaousuario i INNER JOIN sispacto2.tipoperfil t on t.iusd = i.iusd where t.pflcod=1120 and iustipoorientador!='professorsispacto2013' and iustipoorientador!='orientadorsispacto2013' and i.iuscpf in(
		
		select i.iuscpf FROM sispacto.identificacaousuario i 
		inner join sispacto.tipoperfil t on t.iusd = i.iusd and t.pflcod=849 
		inner join sispacto.mensario m on m.iusd = i.iusd 
		inner join sispacto.mensarioavaliacoes ma on ma.menid = m.menid 
		where mavrecomendadocertificacao=1
		
		)
		)";

$db->executar($sql);
$db->commit();

$sql = "UPDATE sispacto2.identificacaousuario set picid=null where iusd in(
		
		select i.iusd FROM sispacto2.identificacaousuario i 
		inner join sispacto2.tipoperfil t on t.iusd = i.iusd 
		inner join seguranca.perfil p on p.pflcod = t.pflcod
		where t.pflcod not in(1119,1120,1118) and picid is not null
		
		)";

$db->executar($sql);
$db->commit();

$sql = "UPDATE sispacto2.identificacaousuario x set picid=foo.picid FROM (

		select i.iusd, i2.picid FROM sispacto2.identificacaousuario i 
		inner join sispacto2.tipoperfil t on t.iusd = i.iusd and t.pflcod=1118 
		left join sispacto2.orientadorturma ot on ot.iusd = i.iusd 
		inner join sispacto2.turmas tu on tu.turid = ot.turid 
		inner join sispacto2.identificacaousuario i2 on i2.iusd = tu.iusd 
		inner join sispacto2.tipoperfil t2 on t2.iusd = i2.iusd and t2.pflcod=1120 
		where i.picid != i2.picid
		
		) foo where x.iusd = foo.iusd";

$db->executar($sql);
$db->commit();

$sql = "UPDATE sispacto2.identificacaousuario x set muncodatuacao=foo.muncod FROM (
		select i.iusd, m.muncod FROM sispacto2.identificacaousuario i
		inner join sispacto2.tipoperfil t on t.iusd = i.iusd and pflcod in(1120, 1118, 1119) 
		inner join sispacto2.pactoidadecerta p on p.picid = i.picid 
		inner join territorios.municipio m on m.muncod = p.muncod
		where muncodatuacao is null
		) foo where x.iusd = foo.iusd";

$db->executar($sql);
$db->commit();

$sql = "UPDATE sispacto2.identificacaousuario x set muncodatuacao=foo.muncodatuacao FROM (
		select i.iusd, i2.muncodatuacao FROM sispacto2.identificacaousuario i 
		inner join sispacto2.tipoperfil t on t.iusd = i.iusd and t.pflcod=1118
		inner join sispacto2.orientadorturma o on o.iusd = i.iusd 
		inner join sispacto2.turmas tu on tu.turid = o.turid 
		inner join sispacto2.identificacaousuario i2 on i2.iusd = tu.iusd 
		inner join sispacto2.tipoperfil t2 on t2.iusd = i2.iusd and t2.pflcod=1120
		where i.iusstatus='A' and i2.iusstatus='A' and i2.iusformacaoinicialorientador=true and i.muncodatuacao is null and i2.muncodatuacao is not null) foo
		where x.iusd = foo.iusd";

$db->executar($sql);
$db->commit();

$sql = "UPDATE sispacto2.identificacaousuario x set muncodatuacao=foo.muncod FROM (
		select i.iusd, i2.muncod FROM sispacto2.identificacaousuario i
		inner join sispacto2.tipoperfil t on t.iusd = i.iusd and pflcod in(1118) 
		inner join sispacto2.pactoidadecerta p on p.picid = i.picid 
		inner join sispacto2.orientadorturma ot on ot.iusd = i.iusd 
		inner join sispacto2.turmas tu on tu.turid = ot.turid 
		inner join sispacto2.identificacaousuario i2 on i2.iusd = tu.iusd
		where i.muncodatuacao is null and i2.muncod is not null
		) foo where x.iusd = foo.iusd";

$db->executar($sql);
$db->commit();

$sql = "UPDATE sispacto2.identificacaousuario x set muncodatuacao=foo.muncod FROM (
		select i.iusd, u.muncod FROM sispacto2.identificacaousuario i 
		inner join sispacto2.tipoperfil t on i.iusd = t.iusd 
		inner join sispacto2.universidadecadastro c on c.uncid = i.uncid 
		inner join sispacto2.universidade u on u.uniid = c.uniid 
		inner join sispacto2.pactoidadecerta p on p.picid = i.picid 
		inner join territorios.municipio m on m.muncod = u.muncod 
		inner join territorios.municipio m2 on m2.muncod = i.muncodatuacao
		where t.pflcod in(1118,1120) and m2.estuf != m.estuf and p.estuf is not null
		) foo where x.iusd = foo.iusd";

$db->executar($sql);
$db->commit();

$sql = "UPDATE sispacto2.identificacaousuario x set muncodatuacao=foo.muncod FROM (
		select i.iusd, uu.muncod FROM sispacto2.identificacaousuario i 
		inner join sispacto2.universidadecadastro un on un.uncid = i.uncid 
		inner join sispacto2.universidade uu on uu.uniid = un.uniid 
		where muncodatuacao is null
		) foo where x.iusd=foo.iusd
		";

$db->executar($sql);
$db->commit();


$sql = "UPDATE sispacto2.identificacaousuario x set uncid=xx.ecu FROM (
		select i.uncid as ius, e.uncid as ecu, i.iusd FROM sispacto2.identificacaousuario i 
		inner join sispacto2.tipoperfil t on t.iusd = i.iusd
		inner join sispacto2.pactoidadecerta p on p.picid = i.picid 
		inner join sispacto2.abrangencia a on a.muncod = p.muncod and esfera='M'
		inner join sispacto2.estruturacurso e on e.ecuid = a.ecuid 
		inner join sispacto2.universidadecadastro u on u.uncid = e.uncid 
		inner join workflow.documento d on d.docid = u.docid 
		where (i.uncid is null or i.uncid != e.uncid) and d.esdid=993
		) xx where xx.iusd = x.iusd";

$db->executar($sql);
$db->commit();


$sql = "UPDATE sispacto2.identificacaousuario x set uncid=xx.ecu FROM (
	
		select i.uncid as ius, e.uncid as ecu, i.iusd FROM sispacto2.identificacaousuario i 
		inner join sispacto2.tipoperfil t on t.iusd = i.iusd
		inner join sispacto2.pactoidadecerta p on p.picid = i.picid and p.estuf is not null
		inner join territorios.municipio m on m.muncod = i.muncodatuacao 
		inner join sispacto2.abrangencia a on a.muncod = m.muncod and esfera='E'
		inner join sispacto2.estruturacurso e on e.ecuid = a.ecuid 
		inner join sispacto2.universidadecadastro u on u.uncid = e.uncid 
		inner join workflow.documento d on d.docid = u.docid 
		where i.uncid is null and d.esdid=993
		
		) xx where xx.iusd = x.iusd";


$db->executar($sql);
$db->commit();

$sql = "UPDATE sispacto2.identificacaousuario x set uncid=foo.uncid FROM (
			select i.uncid, i2.iusd FROM sispacto2.identificacaousuario i
			inner join sispacto2.pactoidadecerta p on i.picid = p.picid 
			inner join sispacto2.tipoperfil t1 on t1.iusd = i.iusd and t1.pflcod=1120
			inner join sispacto2.turmas t on t.iusd = i.iusd 
			inner join sispacto2.orientadorturma ot on ot.turid = t.turid 
			inner join sispacto2.identificacaousuario i2 on i2.iusd = ot.iusd 
			inner join sispacto2.tipoperfil t2 on t2.iusd = i2.iusd and t2.pflcod=1118
			where i.uncid!=i2.uncid
			) foo where x.iusd = foo.iusd";

$db->executar($sql);
$db->commit();

$sql = "DELETE FROM sispacto2.impressoesana WHERE imaid IN(
		SELECT imaid FROM sispacto2.impressoesana i 
		INNER JOIN sispacto2.turmasprofessoresalfabetizadores pa ON pa.tpaid = i.tpaid 
		WHERE pa.tpastatus='I'
		)";

$db->executar($sql);
$db->commit();


$sql = "UPDATE sispacto2.mensarioavaliacoes ma SET mavtotal=foo.total FROM (
			select * FROM (
			select
			mavid,
			mavfrequencia,
			mavatividadesrealizadas,
			mavmonitoramento,
			mavtotal,
			(coalesce((mavfrequencia*fatfrequencia),0) + coalesce((mavatividadesrealizadas*fatatividadesrealizadas),0) + coalesce(mavmonitoramento,0)) as total
			FROM sispacto2.mensarioavaliacoes ma
			inner join sispacto2.mensario m ON m.menid = ma.menid
			inner join sispacto2.identificacaousuario u ON u.iusd = m.iusd
			inner join sispacto2.tipoperfil t ON t.iusd = u.iusd
			inner join sispacto2.fatoresdeavaliacao f ON f.fatpflcodavaliado = t.pflcod
			) fee
			where fee.mavtotal != total
			) foo
			where ma.mavid = foo.mavid";

$db->executar($sql);
$db->commit();


$sql = "UPDATE sispacto2.mensarioavaliacoes x set mavmonitoramento=foo.fatmonitoramento FROM (

			select mm.*, f.*, d.esdid FROM sispacto2.mensario m
			inner join sispacto2.tipoperfil t on t.iusd = m.iusd and t.pflcod!=".PFL_PROFESSORALFABETIZADOR."
			INNER JOIN sispacto2.fatoresdeavaliacao f ON f.fatpflcodavaliado = t.pflcod
			inner join workflow.documento d on d.docid = m.docid and d.esdid in(".ESD_APROVADO_MENSARIO.",".ESD_ENVIADO_MENSARIO.")
			inner join sispacto2.mensarioavaliacoes mm on mm.menid = m.menid
			where mavmonitoramento=0
		
			) foo where foo.mavid = x.mavid";

$db->executar($sql);
$db->commit();


$sql = "UPDATE sispacto2.mensario x set pflcod=(select pflcod FROM sispacto2.tipoperfil where iusd=foo.iusd) FROM (
		select iusd, menid FROM sispacto2.mensario where pflcod is null
		) foo
		where x.menid = foo.menid";

$db->executar($sql);
$db->commit();

$sql = "UPDATE sispacto2.mensarioavaliacoes x set pflcodavaliador=(select pflcod FROM sispacto2.tipoperfil where iusd=foo.iusdavaliador) from (
		select mavid, iusdavaliador from sispacto2.mensarioavaliacoes where pflcodavaliador is null
		) foo
		where x.mavid = foo.mavid";

$db->executar($sql);
$db->commit();


// REMOVENDO OS ARQUIVOS TEMPORARIOS

$dir    = DIRFILES . 'sispacto2/files_tmp';
$files1 = scandir($dir);

if(count($files1) > 2) {
	foreach($files1 as $fl) {

		if(is_file($dir.'/'.$fl)) {
			$res = unlink($dir.'/'.$fl);
			echo $dir.'/'.$fl.' '.(($res)?'SUCESSO':'ERRO').'<br>';
		}

	}
}

carregarTurmasCoordenadorLocal(array());


$sql = "DELETE from sispacto2.orientadorturma where otuid in(
select ot.otuid from sispacto2.orientadorturma ot
inner join sispacto2.turmas tu ON tu.turid = ot.turid
inner join sispacto2.identificacaousuario i ON i.iusd = tu.iusd
inner join sispacto2.identificacaousuario i2 ON i2.iusd = ot.iusd
inner join sispacto2.pactoidadecerta p ON p.picid = i2.picid and p.muncod is not null
where i.uncid != i2.uncid
)";

$db->executar($sql);
$db->commit();

$sql = "DELETE from sispacto2.orientadorturmaoutros where otuid in(
select ot.otuid from sispacto2.orientadorturmaoutros ot
inner join sispacto2.turmas tu ON tu.turid = ot.turid
inner join sispacto2.identificacaousuario i ON i.iusd = tu.iusd
inner join sispacto2.identificacaousuario i2 ON i2.iusd = ot.iusd
inner join sispacto2.pactoidadecerta p ON p.picid = i2.picid and p.muncod is not null
where i.uncid != i2.uncid
)";

$db->executar($sql);
$db->commit();

$sql = "DELETE from sispacto2.orientadorturmaoutros where otuid in (
select otuid from sispacto2.orientadorturmaoutros ot
inner join sispacto2.turmas tu ON tu.turid = ot.turid
inner join sispacto2.identificacaousuario i ON i.iusd = tu.iusd
left join sispacto2.tipoperfil t ON t.iusd = i.iusd
where t.tpeid is null
)";

$db->executar($sql);
$db->commit();

$sql = "DELETE from sispacto2.historicoreaberturanota where mavid in (

	select ma.mavid from sispacto2.mensarioavaliacoes ma
	inner join sispacto2.mensario m ON m.menid = ma.menid
	inner join sispacto2.orientadorturma o ON o.iusd = m.iusd
	inner join sispacto2.turmas t ON t.turid = o.turid
	where m.pflcod=1118 and t.iusd!=ma.iusdavaliador and ma.menid in(
	select foo.menid from (
	select m.menid, (select count(*) from sispacto2.mensarioavaliacoes where menid=m.menid) as tt from sispacto2.mensario m
	inner join sispacto2.tipoperfil t ON t.iusd = m.iusd
	inner join workflow.documento d ON d.docid = m.docid
	where t.pflcod=1118 and d.esdid!=989
	) foo where foo.tt > 1
	)

	)";

$db->executar($sql);
$db->commit();


$sql = "DELETE FROM sispacto2.mensarioavaliacoes WHERE mavid IN (

	SELECT ma.mavid FROM sispacto2.mensarioavaliacoes ma
	INNER JOIN sispacto2.mensario m ON m.menid = ma.menid
	INNER JOIN sispacto2.orientadorturma o ON o.iusd = m.iusd
	INNER JOIN sispacto2.turmas t ON t.turid = o.turid
	WHERE m.pflcod=".PFL_PROFESSORALFABETIZADOR." AND t.iusd!=ma.iusdavaliador AND ma.menid IN(
		
	SELECT foo.menid FROM (
		
	SELECT m.menid, (SELECT count(*) FROM sispacto2.mensarioavaliacoes WHERE menid=m.menid) as tt FROM sispacto2.mensario m
	INNER JOIN sispacto2.tipoperfil t ON t.iusd = m.iusd
	INNER JOIN workflow.documento d ON d.docid = m.docid
	WHERE t.pflcod=".PFL_PROFESSORALFABETIZADOR." AND d.esdid!=".ESD_APROVADO_MENSARIO."
		
	) foo WHERE foo.tt > 1
		
	)

	)";

$db->executar($sql);
$db->commit();

// REMOVENDO USUARIOS QUE POSSUEM MAIS DE UM PERFIL DE BOLSISTA NO SIMEC (SOMENTE PARA ACESSO AO SIMEC)

$sql = "DELETE FROM seguranca.perfilusuario p
		USING (
		select iuscpf, pf.pflcod from sispacto2.identificacaousuario i
		inner join seguranca.perfilusuario pp ON pp.usucpf = i.iuscpf
		inner join seguranca.perfil p ON p.pflcod = pp.pflcod and p.sisid=181
		inner join sispacto2.pagamentoperfil pf ON pf.pflcod = p.pflcod
		inner join sispacto2.tipoperfil t ON t.iusd = i.iusd
		where i.iusstatus='A' and pf.pflcod!=t.pflcod
		) foo WHERE p.usucpf=foo.iuscpf AND foo.pflcod=p.pflcod";

$db->executar($sql);
$db->commit();


// CRIANDO TURMAS DOS FORMADORES QUE SÃO INCLUIDOS DEPOIS DO PROJETO VALIDADO

$sql = "INSERT INTO sispacto2.turmas(
uncid, iusd, turdesc, turstatus, picid, muncod)

select i.uncid, i.iusd, 'TURMA FR - '||i.iusnome as turma, 'A', null, null from sispacto2.identificacaousuario i 
inner join sispacto2.tipoperfil t ON t.iusd = i.iusd 
inner join sispacto2.universidadecadastro u ON u.uncid = i.uncid 
inner join workflow.documento d ON d.docid = u.docid 
left join sispacto2.turmas tu ON tu.iusd = i.iusd 
where i.iusstatus='A' AND t.pflcod=1131 and d.esdid=993 and tu.turid is null";

$db->executar($sql);
$db->commit();


$sql = "UPDATE sispacto2.identificacaousuario f set iustipoprofessor=foo.tipop from (

	select i.iusd, i.iuscpf, i.iustipoprofessor, case when p.cpf is null then 'cpflivre' else 'censo' end as tipop from sispacto2.identificacaousuario i
	inner join sispacto2.tipoperfil t ON t.iusd = i.iusd and pflcod=1118
	left join sispacto2.professoresalfabetizadores p ON p.cpf = i.iuscpf
	where (i.iustipoprofessor != case when p.cpf is null then 'cpflivre' else 'censo' end or i.iustipoprofessor is null) and i.iusstatus='A'

	) foo where foo.iusd = f.iusd and foo.tipop='censo'";

$db->executar($sql);
$db->commit();


$sql = "DELETE FROM sispacto2.certificacao";
$db->executar($sql);
$db->commit();

$sql = "INSERT INTO sispacto2.certificacao(
		iusd, pflcod, cerfrequencia)
		select foo.iusd, foo.pflcod, case when foo.freq > 100 then 100.0 else foo.freq end as cerfrequencia from (
		select i.iusd, round((sum(ma.mavfrequencia)/(select count(*) from sispacto2.folhapagamentouniversidade where pflcod=m.pflcod and uncid=i.uncid))*100,1) as freq, m.pflcod from sispacto2.mensario m
		inner join sispacto2.mensarioavaliacoes ma on ma.menid = m.menid and ma.pflcodavaliador=1131
		inner join sispacto2.identificacaousuario i on i.iusd = m.iusd
		where m.pflcod=1120 and i.uncid is not null
		group by i.iusd,m.pflcod,i.uncid
		) foo";

$db->executar($sql);
$db->commit();

$sql = "INSERT INTO sispacto2.certificacao(
		iusd, pflcod, cerfrequencia)
		select i.iusd, t.pflcod, '0.0' as freq from sispacto2.identificacaousuario i
		inner join sispacto2.tipoperfil t on t.iusd = i.iusd and t.pflcod=1120
		left join sispacto2.certificacao c on c.iusd = i.iusd
		where c.cerfrequencia is null and i.uncid is not null and i.iuscpf not ilike 'SIS%'";

$db->executar($sql);
$db->commit();

$sql = "INSERT INTO sispacto2.certificacao(
		iusd, pflcod, cerfrequencia)
		select foo.iusd, foo.pflcod, case when foo.freq > 100 then 100.0 else foo.freq end as cerfrequencia from (
		select i.iusd, round((sum(ma.mavfrequencia)/(select count(*) from sispacto2.folhapagamentouniversidade where pflcod=m.pflcod and uncid=i.uncid))*100,1) as freq, m.pflcod from sispacto2.mensario m
		inner join sispacto2.mensarioavaliacoes ma on ma.menid = m.menid and ma.pflcodavaliador=1120
		inner join sispacto2.identificacaousuario i on i.iusd = m.iusd
		where m.pflcod=1118 and i.uncid is not null
		group by i.iusd,m.pflcod,i.uncid
		) foo";

$db->executar($sql);
$db->commit();

$sql = "INSERT INTO sispacto2.certificacao(
		iusd, pflcod, cerfrequencia)
		select i.iusd, t.pflcod, 0.0 as freq from sispacto2.identificacaousuario i
		inner join sispacto2.tipoperfil t on t.iusd = i.iusd and t.pflcod=1118
		left join sispacto2.certificacao c on c.iusd = i.iusd
		where i.iusd is null and i.uncid is not null and i.iuscpf not ilike 'SIS%'";

$db->executar($sql);
$db->commit();

$sql = "UPDATE sispacto2.certificacao SET cerfrequencia=0.0 WHERE cerfrequencia IS NULL";
$db->executar($sql);
$db->commit();


$sql = "UPDATE seguranca.agendamentoscripts SET agstempoexecucao='".round((getmicrotime() - $microtime),2)."' WHERE agsfile='sispacto2_scripts_manutencao.php'";
$db->executar($sql);
$db->commit();


$db->close();


?>