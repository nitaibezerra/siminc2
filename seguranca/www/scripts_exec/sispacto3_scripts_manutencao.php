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
require_once APPRAIZ . "www/sispacto3/_constantes.php";
require_once APPRAIZ . "www/sispacto3/_funcoes.php";

require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';


// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';

function getmicrotime() {list($usec, $sec) = explode(" ", microtime()); return ((float)$usec + (float)$sec);}

$microtime = getmicrotime();
   
// abre conexção com o servidor de banco de dados
$db = new cls_banco();

$sql = "delete from sispacto3.orientadorestudoturmacl where iusd in(
SELECT i.iusd
			FROM sispacto3.identificacaousuario i 
			INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd 
			INNER JOIN sispacto3.orientadorestudoturmacl ot ON ot.iusd = i.iusd
			INNER JOIN sispacto3.turmas tt ON tt.turid = ot.turid 
			INNER JOIN sispacto3.identificacaousuario i2 ON i2.iusd = tt.iusd 
			WHERE t.pflcod=1381 AND i.iusstatus='A' AND i.picid!=i2.picid
)";

$db->executar($sql);
$db->commit();

$sql = "delete from sispacto3.turmas where turid in(
		select foo.turid from (
		select 
		turid, 
		(select count(*) from sispacto3.professoralfabetizadorturma where turid=t.turid) as npa,
		(select count(*) from sispacto3.orientadorestudoturmacl where turid=t.turid) as noecl 
		
		from sispacto3.turmas t where picid is not null and turdesc ilike 'Turma OE%'
		) foo 
		where foo.npa=0 and foo.noecl=0
		)";

$db->executar($sql);
$db->commit();


$sql = "update sispacto3.identificacaousuario set iusstatus='A' where iusd in(
		select i.iusd from sispacto3.identificacaousuario i 
		inner join sispacto3.tipoperfil t on t.iusd = i.iusd 
		where i.iusstatus='I' and i.iuscpf not ilike 'REM%'
		)";

$db->executar($sql);
$db->commit();


$sql = "delete from sispacto3.orientadorestudoturmacl where turid in(
		select tu.turid from sispacto3.identificacaousuario i 
		left join sispacto3.tipoperfil t on t.iusd = i.iusd 
		inner join sispacto3.turmas tu on tu.iusd = i.iusd
		where t.tpeid is null
		)";

$db->executar($sql);
$db->commit();



$sql = "update sispacto3.identificacaousuario x set iustipoorientador=foo.tipoorientador from (

		select 
		i.iusd,  
		case when c1.cerfrequencia>=75 then 'orientadorsispacto2014' 
		     when c2.cerfrequencia>=75 then 'professorsispacto2014' 
		     when prol.cpf is not null then 'tutoresproletramento' 
		     when srol.cpf is not null then 'tutoresredesemproletramento' 
		     else 'profissionaismagisterio' end as tipoorientador
		
		
		from sispacto3.identificacaousuario i 
		inner join sispacto3.tipoperfil t on t.iusd = i.iusd 
		left join sispacto2.identificacaousuario i2 on i2.iuscpf = i.iuscpf 
		left join sispacto2.certificacao c1 on c1.iusd = i2.iusd and c1.pflcod=1120
		left join sispacto2.certificacao c2 on c2.iusd = i2.iusd and c1.pflcod=1118 
		left join sispacto3.tutoresproletramento prol on prol.cpf = i.iuscpf 
		left join sispacto3.tutoressemproletramento srol on srol.cpf = i.iuscpf
		where i.iusstatus='A' and t.pflcod=1381
		
		) foo where x.iusd=foo.iusd";

$db->executar($sql);
$db->commit();

$sql = "update sispacto3.identificacaousuario x set iustipoprofessor=foo.iustipoprofessor from (

		select 
		i.iusd,  
		case when p.cpf is not null then 'censo'
		     else 'cpflivre' end as iustipoprofessor
		from sispacto3.identificacaousuario i 
		inner join sispacto3.tipoperfil t on t.iusd = i.iusd 
		left join sispacto3.professoresalfabetizadores p on p.cpf = i.iuscpf 
		where i.iusstatus='A' and t.pflcod=1379
		
		) foo where x.iusd=foo.iusd and x.iustipoprofessor!=foo.iustipoprofessor";

$db->executar($sql);
$db->commit();


$sql = "DELETE from sispacto3.professoralfabetizadorturma where otuid in(
select ot.otuid from sispacto3.identificacaousuario i 
inner join sispacto3.tipoperfil t on t.iusd = i.iusd 
inner join sispacto3.professoralfabetizadorturma ot on ot.iusd = i.iusd 
inner join sispacto3.turmas tu on tu.turid = ot.turid 
inner join sispacto3.identificacaousuario i2 on i2.iusd = tu.iusd 
inner join sispacto3.tipoperfil t2 on t2.iusd = i2.iusd 
where t.pflcod=1379 and t2.pflcod=1381 and i.picid!=i2.picid
)";

$db->executar($sql);
$db->commit();

// ATUALIZANDO A IES NA QUAL PARTICIPA NO CURSO

$sql = "UPDATE sispacto3.identificacaousuario x set uncid=xx.ecu FROM (
select i.uncid as ius, e.uncid as ecu, i.iusd FROM sispacto3.identificacaousuario i
inner join sispacto3.tipoperfil t on t.iusd = i.iusd
inner join sispacto3.pactoidadecerta p on p.picid = i.picid
inner join sispacto3.abrangencia a on a.muncod = p.muncod and esfera='M'
		inner join sispacto3.estruturacurso e on e.ecuid = a.ecuid
		inner join sispacto3.universidadecadastro u on u.uncid = e.uncid
		inner join workflow.documento d on d.docid = u.docidestruturaformacao
		where case when t.pflcod=".PFL_COORDENADORLOCAL." then i.uncid is null else (i.uncid is null or i.uncid != e.uncid) end and d.esdid=".ESD_VALIDADO_COORDENADOR_IES."
) xx where xx.iusd = x.iusd";

$db->executar($sql);
$db->commit();

$sql = "UPDATE sispacto3.identificacaousuario x set uncid=xx.ecu FROM (

select i.uncid as ius, e.uncid as ecu, i.iusd FROM sispacto3.identificacaousuario i
inner join sispacto3.tipoperfil t on t.iusd = i.iusd
inner join sispacto3.pactoidadecerta p on p.picid = i.picid and p.estuf is not null
inner join territorios.municipio m on m.muncod = i.muncodatuacao
inner join sispacto3.abrangencia a on a.muncod = m.muncod and esfera='E'
		inner join sispacto3.estruturacurso e on e.ecuid = a.ecuid
		inner join sispacto3.universidadecadastro u on u.uncid = e.uncid
		inner join workflow.documento d on d.docid = u.docidestruturaformacao
		where case when t.pflcod=".PFL_COORDENADORLOCAL." then i.uncid is null else (i.uncid is null or i.uncid != e.uncid) end and d.esdid=".ESD_VALIDADO_COORDENADOR_IES."

) xx where xx.iusd = x.iusd";

$db->executar($sql);
$db->commit();

$sql = "UPDATE sispacto3.identificacaousuario x set uncid=foo.uncid FROM (
			select i.uncid, i2.iusd FROM sispacto3.identificacaousuario i
			inner join sispacto3.pactoidadecerta p on i.picid = p.picid
			inner join sispacto3.tipoperfil t1 on t1.iusd = i.iusd and t1.pflcod=".PFL_ORIENTADORESTUDO."
			inner join sispacto3.turmas t on t.iusd = i.iusd
			inner join sispacto3.professoralfabetizadorturma ot on ot.turid = t.turid
			inner join sispacto3.identificacaousuario i2 on i2.iusd = ot.iusd
			inner join sispacto3.tipoperfil t2 on t2.iusd = i2.iusd and t2.pflcod=".PFL_PROFESSORALFABETIZADOR."
			where (i.uncid!=i2.uncid or i2.uncid is null)
			) foo where x.iusd = foo.iusd";

$db->executar($sql);
$db->commit();


// ATUALIZANDO O MUNICIPIO DE ATUAÇÃO

$sql = "UPDATE sispacto3.identificacaousuario x set muncodatuacao=foo.muncod FROM (
		
		select i.iusd, m.muncod FROM sispacto3.identificacaousuario i
		inner join sispacto3.tipoperfil t on t.iusd = i.iusd and pflcod in(".PFL_PROFESSORALFABETIZADOR.", ".PFL_ORIENTADORESTUDO.", ".PFL_COORDENADORLOCAL.")
		inner join sispacto3.pactoidadecerta p on p.picid = i.picid
		inner join territorios.municipio m on m.muncod = p.muncod
		where muncodatuacao is null
				
		) foo where x.iusd = foo.iusd";

$db->executar($sql);
$db->commit();

$sql = "UPDATE sispacto3.identificacaousuario x set muncodatuacao=foo.muncodatuacao FROM (
		select i.iusd, i2.muncodatuacao FROM sispacto3.identificacaousuario i
		inner join sispacto3.tipoperfil t on t.iusd = i.iusd and t.pflcod=".PFL_PROFESSORALFABETIZADOR."
		inner join sispacto3.professoralfabetizadorturma o on o.iusd = i.iusd
		inner join sispacto3.turmas tu on tu.turid = o.turid
		inner join sispacto3.identificacaousuario i2 on i2.iusd = tu.iusd
		inner join sispacto3.tipoperfil t2 on t2.iusd = i2.iusd and t2.pflcod=".PFL_ORIENTADORESTUDO."
		where i.iusstatus='A' and i2.iusstatus='A' and i2.iusformacaoinicialorientador=true and i.muncodatuacao is null and i2.muncodatuacao is not null) foo
		where x.iusd = foo.iusd";

$db->executar($sql);
$db->commit();

$sql = "UPDATE sispacto3.identificacaousuario x set muncodatuacao=foo.muncod FROM (
		select i.iusd, i2.muncod FROM sispacto3.identificacaousuario i
		inner join sispacto3.tipoperfil t on t.iusd = i.iusd and pflcod in(".PFL_PROFESSORALFABETIZADOR.")
		inner join sispacto3.pactoidadecerta p on p.picid = i.picid
		inner join sispacto3.professoralfabetizadorturma ot on ot.iusd = i.iusd
		inner join sispacto3.turmas tu on tu.turid = ot.turid
		inner join sispacto3.identificacaousuario i2 on i2.iusd = tu.iusd
		where i.muncodatuacao is null and i2.muncod is not null
		) foo where x.iusd = foo.iusd";

$db->executar($sql);
$db->commit();

$sql = "UPDATE sispacto3.identificacaousuario x set muncodatuacao=foo.muncod FROM (
		select i.iusd, u.muncod FROM sispacto3.identificacaousuario i
		inner join sispacto3.tipoperfil t on i.iusd = t.iusd
		inner join sispacto3.universidadecadastro c on c.uncid = i.uncid
		inner join sispacto3.universidade u on u.uniid = c.uniid
		inner join sispacto3.pactoidadecerta p on p.picid = i.picid
		inner join territorios.municipio m on m.muncod = u.muncod
		inner join territorios.municipio m2 on m2.muncod = i.muncodatuacao
		where t.pflcod in(".PFL_PROFESSORALFABETIZADOR.", ".PFL_ORIENTADORESTUDO.") and m2.estuf != m.estuf and p.estuf is not null
		) foo where x.iusd = foo.iusd";

$db->executar($sql);
$db->commit();

// LIMPANDO VINCULO MUNICIPAL/ ESTADUAL DA EQUIPE IES

$sql = "update sispacto3.identificacaousuario set picid=null where iusd in(

		select i.iusd from sispacto3.identificacaousuario i 
		inner join sispacto3.tipoperfil t on t.iusd = i.iusd 
		inner join seguranca.perfil p on p.pflcod = t.pflcod 
		where t.pflcod in(".PFL_COORDENADORIES.",".PFL_FORMADORIES.",".PFL_SUPERVISORIES.",".PFL_COORDENADORADJUNTOIES.")
		
		)";

$db->executar($sql);
$db->commit();

// LIMPANDO VINCULO COM IES QUE NÃO TENHA MUNICIPIO NA ESTRUTURA DA FORMAÇÃO

$sql = "update sispacto3.identificacaousuario set uncid=null where iusd in(
select i.iusd from territorios.municipio m 
left join sispacto3.abrangencia a on a.muncod = m.muncod and a.esfera='M' 
left join sispacto3.estruturacurso e on e.ecuid = a.ecuid 
left join sispacto3.pactoidadecerta p on p.muncod = m.muncod 
left join sispacto3.identificacaousuario i on i.picid = p.picid
where a.abrid is null and i.uncid is not null
)";

$db->executar($sql);
$db->commit();

// VALIDANDO MUNICIPIOS CUJO AS IES ESTÃO VALIDADAS

$sql = "select distinct p.picid, p.docid from sispacto3.universidadecadastro u 
		inner join sispacto3.estruturacurso e on e.uncid = u.uncid 
		inner join sispacto3.abrangencia a on a.ecuid = e.ecuid and esfera='M'
		inner join sispacto3.pactoidadecerta p on p.muncod = a.muncod
		inner join workflow.documento d on d.docid = u.docidformacaoinicial 
		inner join workflow.documento d2 on d2.docid = p.docid 
		inner join workflow.documento d3 on d3.docid = p.docidturma 
		where d.esdid=".ESD_FECHADO_FORMACAOINICIAL." and d2.esdid=".ESD_ANALISE_COORDENADOR_LOCAL." and d3.esdid=".ESD_FECHADO_TURMA; 

$universidadecadastro = $db->carregar($sql); 

if($universidadecadastro[0]) {
	foreach($universidadecadastro as $uc) {
		$result = wf_alterarEstado( $uc['docid'], AED_VALIDAR_COORDENADORLOCAL, '', array());
		echo $result."<br>";
	}
}

// AVALIANDO CGs QUE TENHA EFETUADO TODO PROJETO

$sql = "select i.iusd, i.iusnome, fu.fpbid, m.menid, ma.mavid FROM sispacto3.identificacaousuario i
		inner join sispacto3.tipoperfil t on t.iusd = i.iusd
		inner join sispacto3.universidadecadastro u on u.uncid = i.uncid
		inner join workflow.documento d on d.docid = u.docidorcamento
		inner join workflow.documento d2 on d2.docid = u.docidestruturaformacao
		inner join workflow.documento d3 on d3.docid = u.docidequipeies 
		inner join workflow.documento d4 on d4.docid = u.dociddadosprojeto 
		inner join workflow.documento d5 on d5.docid = u.docidturmas
			  
		inner join workflow.documento dfi on dfi.docid = u.docidformacaoinicial
		inner join sispacto3.folhapagamentouniversidade fu on fu.uncid = i.uncid and fu.pflcod = t.pflcod
		inner join sispacto3.folhapagamento f on f.fpbid = fu.fpbid
		left join sispacto3.mensario m on m.iusd = i.iusd and m.fpbid = fu.fpbid
		left join sispacto3.mensarioavaliacoes ma on ma.menid = m.menid
		where d.esdid='".ESD_VALIDADO_COORDENADOR_IES."' and 
			  d2.esdid='".ESD_VALIDADO_COORDENADOR_IES."' and 
			  d3.esdid='".ESD_VALIDADO_COORDENADOR_IES."' and 
			  d4.esdid='".ESD_VALIDADO_COORDENADOR_IES."' and 
			  d5.esdid='".ESD_VALIDADO_COORDENADOR_IES."' and 
			  u.usucpfparecer IS NOT NULL and 
			  i.iusstatus='A' and 
			  ma.mavid is null and 
			  t.pflcod=".PFL_COORDENADORIES." and 
			  dfi.esdid=".ESD_FECHADO_FORMACAOINICIAL." and 
			  to_char(NOW(),'YYYY-mm-dd')>=to_char((fpbanoreferencia::text||'-'||lpad(fpbmesreferencia::text, 2, '0')||'-15')::date,'YYYY-mm-dd')";

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

		$sql = "INSERT INTO sispacto3.mensarioavaliacoes(
				iusdavaliador, menid, mavatividadesrealizadas, mavmonitoramento, mavtotal)
				VALUES ('".IUS_AVALIADOR_MEC."', '".$res['memid']."', '1.0', '{$mavmonitoramento}', '{$mavtotal}');";

		$db->executar($sql);

	}

	$db->commit();
}

// ATUALIZANDO NOTA FINAL DE BOLSISTAS CONFORME AS AVALIAÇÕES E O PERFIL NO MOMENTO DA AVALIAÇÃO (MUITO COMUM EM BOLSISTAS QUE MUDAM DE PERFIL DURANTE O PROGRAMA)

$fpbids = $db->carregarColuna("SELECT fpbid FROM sispacto3.folhapagamento");

if($fpbids) {
	foreach($fpbids as $fpbid) {

		$sql = "UPDATE sispacto3.mensarioavaliacoes ma SET mavtotal=foo.total FROM (
		select
		mavid,
		mavfrequencia,
		mavatividadesrealizadas,
		mavmonitoramento,
		mavtotal,
		(coalesce((mavfrequencia*fatfrequencia),0) + coalesce((mavatividadesrealizadas*fatatividadesrealizadas),0) + coalesce(mavmonitoramento,0)) as total
		FROM sispacto3.mensario m
		inner join sispacto3.mensarioavaliacoes ma ON m.menid = ma.menid
		inner join workflow.documento d ON d.docid = m.docid
		inner join sispacto3.fatoresdeavaliacao f ON f.fatpflcodavaliado = m.pflcod
		where d.esdid!=".ESD_PAGAMENTO_EFETIVADO." and m.fpbid={$fpbid} and ((coalesce((mavfrequencia*fatfrequencia),0) + coalesce((mavatividadesrealizadas*fatatividadesrealizadas),0) + coalesce(mavmonitoramento,0)))!=mavtotal
		) foo
		where ma.mavid = foo.mavid";

		$db->executar($sql);
		$db->commit();


	}

}



// ATUALIZA A TABELA DE USUARIO DO SIMEC COM NOME VALIDADO NA RECEITA FEDERAL

$sql = "UPDATE seguranca.usuario x set usunome=foo.iusnome FROM (
select i.iusnome, u.usucpf FROM sispacto3.identificacaousuario i
inner join seguranca.usuario u on u.usucpf = i.iuscpf
where cadastradosgb=true and removeacento(i.iusnome) != removeacento(u.usunome)
) foo where x.usucpf = foo.usucpf";

$db->executar($sql);
$db->commit();


// CORRIGINDO NOME CONTENDO CARACTER ESTRANHO INSERIDO

$sql = "UPDATE sispacto3.identificacaousuario x set iusnome=foo.nome FROM (
select iusd, substr(iusnome,0,strpos(iusnome,'<')) as nome FROM sispacto3.identificacaousuario where iusnome ilike '%<%'
) foo where x.iusd = foo.iusd";

$db->executar($sql);
$db->commit();

// CORRIGINDO EMAILS INVALIDOS

$sql = "UPDATE sispacto3.identificacaousuario set iusemailprincipal=replace(iusemailprincipal,'@com','@meudominio.com') where iusemailprincipal ilike '%@com%';";
$db->executar($sql);
$db->commit();

$sql = "UPDATE sispacto3.identificacaousuario set iusemailprincipal=replace(iusemailprincipal,'@.','@') where iusemailprincipal ilike '%@.%';";
$db->executar($sql);
$db->commit();


// ATUALIZANDO NOME DO CENSO COM NOME VINDO DA RECEITA FEDERAL (SOMENTE SE OS 9 PRIMEIROS DIGITOS FOREM IGUAIS)

$sql = "select distinct i.iusd, s.logresponse FROM sispacto3.identificacaousuario i
		inner join sispacto3.tipoperfil t on t.iusd = i.iusd
		inner join log_historico.logsgb_sispacto3 s on s.logcpf = i.iuscpf and s.logservico='gravarDadosBolsista' and s.logerro=true
		where cadastradosgb=false and iustermocompromisso=true and logresponse ilike '%Erro: 00026:%';";

$arr = $db->carregar($sql);

if($arr[0]) {
	foreach($arr as $ar) {
		$sl = explode("(",$ar['logresponse']);
		$sl = explode(")",$sl[1]);
			
		$iusnome_antigo = $db->pegaUm("select iusnome FROM sispacto3.identificacaousuario where iusd='".$ar['iusd']."'");
			
		// somente atualizar se os 9 primeiros digitos forem semelhantes
		if(substr(strtoupper($iusnome_antigo),0,9)==substr(strtoupper(trim($sl[0])),0,9)) {
			$sql = "UPDATE sispacto3.identificacaousuario set iusnome='".trim($sl[0])."' where iusd='".$ar['iusd']."'";
			$db->executar($sql);
		}
	}
	$db->commit();
}


// CARREGANDO TURMAS DE COORDENADORES LOCAIS

$sql = "INSERT INTO sispacto3.turmas(
            uncid, iusd, turdesc, turstatus, picid, muncod, pflcod)
			SELECT i.uncid, i.iusd, 'TURMA CL #'||i.iusd as turdesc, 'A', i.picid, null, t.pflcod FROM sispacto3.identificacaousuario i
			INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd AND t.pflcod=".PFL_COORDENADORLOCAL."
			LEFT JOIN sispacto3.turmas tu ON tu.iusd = i.iusd  AND tu.pflcod=".PFL_COORDENADORLOCAL."
			WHERE i.iusstatus='A' AND tu.turid IS NULL AND i.picid IN(
		
			SELECT i.picid FROM sispacto3.universidadecadastro u
			INNER JOIN workflow.documento d ON d.docid = u.docid
			INNER JOIN workflow.documento d2 ON d2.docid = u.docidformacaoinicial
			INNER JOIN sispacto3.estruturacurso e ON e.uncid = u.uncid
			INNER JOIN sispacto3.abrangencia a ON a.ecuid = e.ecuid AND a.esfera='M'
			INNER JOIN sispacto3.pactoidadecerta p ON p.muncod = a.muncod
			INNER JOIN sispacto3.identificacaousuario i ON i.picid = p.picid
			INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd AND t.pflcod=".PFL_COORDENADORLOCAL."
			WHERE d.esdid='".ESD_VALIDADO_COORDENADOR_IES."' AND d2.esdid='".ESD_FECHADO_FORMACAOINICIAL."' AND i.iusstatus='A'
			GROUP BY i.picid
				
			)";

$db->executar($sql);

$sql = "INSERT INTO sispacto3.orientadorestudoturmacl(
            turid, iusd, otustatus, otudata)
			SELECT (SELECT max(turid) FROM sispacto3.turmas WHERE pflcod=".PFL_COORDENADORLOCAL." AND picid=i.picid) as turid, i.iusd, 'A', NOW() FROM sispacto3.identificacaousuario i
			INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd AND t.pflcod=".PFL_ORIENTADORESTUDO."
			LEFT JOIN sispacto3.orientadorestudoturmacl ot ON ot.iusd = i.iusd
			WHERE i.iusstatus='A' AND ot.otuid IS NULL AND i.picid IN(
		
			SELECT i.picid FROM sispacto3.universidadecadastro u
			INNER JOIN workflow.documento d ON d.docid = u.docid
			INNER JOIN workflow.documento d2 ON d2.docid = u.docidformacaoinicial
			INNER JOIN sispacto3.estruturacurso e ON e.uncid = u.uncid
			INNER JOIN sispacto3.abrangencia a ON a.ecuid = e.ecuid AND a.esfera='M'
			INNER JOIN sispacto3.pactoidadecerta p ON p.muncod = a.muncod
			INNER JOIN sispacto3.identificacaousuario i ON i.picid = p.picid
			INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd AND t.pflcod=".PFL_COORDENADORLOCAL."
			WHERE d.esdid='".ESD_VALIDADO_COORDENADOR_IES."' AND d2.esdid='".ESD_FECHADO_FORMACAOINICIAL."' AND i.iusstatus='A'
			GROUP BY i.picid
			HAVING count(i.iusd)=1
		
			)";

$db->executar($sql);

$db->commit();

$sql = "INSERT INTO sispacto3.turmas(
            uncid, iusd, turdesc, turstatus, picid, muncod, pflcod)
		SELECT i.uncid, i.iusd, 'TURMA CL #'||i.iusd as turdesc, 'A', i.picid, null, t.pflcod FROM sispacto3.identificacaousuario i
		INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd AND t.pflcod=1380
		LEFT JOIN sispacto3.turmas tu ON tu.iusd = i.iusd  AND tu.pflcod=1380
		WHERE i.iusstatus='A' AND tu.turid IS NULL AND i.picid in(
		
		SELECT p.picid FROM sispacto3.pactoidadecerta p
		INNER JOIN workflow.documento d on d.docid = p.docid
		INNER JOIN workflow.documento d2 on d2.docid = p.docidturma
		WHERE d.esdid=1512 and d2.esdid=1509
		
		)";

$db->executar($sql);

$sql = "INSERT INTO sispacto3.orientadorestudoturmacl(
		turid, iusd, otustatus, otudata)
		SELECT * FROM (
		SELECT (SELECT max(turid) FROM sispacto3.turmas WHERE pflcod=1380 AND picid=i.picid) as turid, i.iusd, 'A', NOW() FROM sispacto3.identificacaousuario i
		INNER JOIN sispacto3.tipoperfil t ON t.iusd = i.iusd AND t.pflcod=1381
		LEFT JOIN sispacto3.orientadorestudoturmacl ot ON ot.iusd = i.iusd
		WHERE i.iusstatus='A' AND ot.otuid IS NULL AND i.picid in(
		
		SELECT p.picid FROM sispacto3.pactoidadecerta p
		INNER JOIN workflow.documento d on d.docid = p.docid
		INNER JOIN workflow.documento d2 on d2.docid = p.docidturma
		WHERE d.esdid=1512 and d2.esdid=1509
		
		)
		) foo 
		WHERE foo.turid IS NOT NULL";

$db->executar($sql);

$db->commit();


$sql = "UPDATE sispacto3.mensarioavaliacoes ma SET mavtotal=foo.total FROM (
			select * FROM (
			select
			mavid,
			mavfrequencia,
			mavatividadesrealizadas,
			mavmonitoramento,
			mavtotal,
			(coalesce((mavfrequencia*fatfrequencia),0) + coalesce((mavatividadesrealizadas*fatatividadesrealizadas),0) + coalesce(mavmonitoramento,0)) as total
			FROM sispacto3.mensarioavaliacoes ma
			inner join sispacto3.mensario m ON m.menid = ma.menid
			inner join sispacto3.identificacaousuario u ON u.iusd = m.iusd
			inner join sispacto3.tipoperfil t ON t.iusd = u.iusd
			inner join sispacto3.fatoresdeavaliacao f ON f.fatpflcodavaliado = t.pflcod
			) fee
			where fee.mavtotal != total
			) foo
			where ma.mavid = foo.mavid";

$db->executar($sql);

$db->commit();


$fpbids = $db->carregarColuna("SELECT fpbid FROM sispacto3.folhapagamento");

if($fpbids) {
	foreach($fpbids as $fpbid) {

		$sql = "UPDATE sispacto3.mensarioavaliacoes ma SET mavtotal=foo.total FROM (
		select
		mavid,
		mavfrequencia,
		mavatividadesrealizadas,
		mavmonitoramento,
		mavtotal,
		(coalesce((mavfrequencia*fatfrequencia),0) + coalesce((mavatividadesrealizadas*fatatividadesrealizadas),0) + coalesce(mavmonitoramento,0)) as total
		FROM sispacto3.mensario m
		inner join sispacto3.mensarioavaliacoes ma ON m.menid = ma.menid
		inner join workflow.documento d ON d.docid = m.docid
		inner join sispacto3.fatoresdeavaliacao f ON f.fatpflcodavaliado = m.pflcod
		where d.esdid!=".ESD_PAGAMENTO_EFETIVADO." and m.fpbid={$fpbid} and ((coalesce((mavfrequencia*fatfrequencia),0) + coalesce((mavatividadesrealizadas*fatatividadesrealizadas),0) + coalesce(mavmonitoramento,0)))!=mavtotal
		) foo
		where ma.mavid = foo.mavid";

		$db->executar($sql);
		$db->commit();


	}

}

$sql = "UPDATE sispacto3.mensarioavaliacoes x set mavmonitoramento=foo.fatmonitoramento FROM (

			select mm.*, f.*, d.esdid FROM sispacto3.mensario m
			inner join sispacto3.tipoperfil t on t.iusd = m.iusd and t.pflcod!=".PFL_PROFESSORALFABETIZADOR."
			INNER JOIN sispacto3.fatoresdeavaliacao f ON f.fatpflcodavaliado = t.pflcod
			inner join workflow.documento d on d.docid = m.docid and d.esdid in(".ESD_APROVADO_MENSARIO.",".ESD_ENVIADO_MENSARIO.")
			inner join sispacto3.mensarioavaliacoes mm on mm.menid = m.menid
			where mavmonitoramento=0

			) foo where foo.mavid = x.mavid";

$db->executar($sql);
$db->commit();


// LIMPANDO TURMAS INCOMPATIVEL POR PERFIL

$sql = "delete from sispacto3.professoralfabetizadorturma where iusd in(
select distinct i.iusd from sispacto3.identificacaousuario i 
inner join sispacto3.tipoperfil t on t.iusd = i.iusd and t.pflcod=1379
inner join sispacto3.professoralfabetizadorturma ot on ot.iusd = i.iusd 
inner join sispacto3.turmas tu on tu.turid = ot.turid 
inner join sispacto3.identificacaousuario i2 on i2.iusd = tu.iusd 
left join sispacto3.tipoperfil t2 on t2.iusd = i2.iusd and t2.pflcod in(1380,1378,1390,1389,1379)
where t2.tpeid is not null
)";

$db->executar($sql);
$db->commit();

$sql = "update sispacto3.identificacaousuario set iustermocompromisso=null where iusd in(
select i.iusd from sispacto3.identificacaousuario i 
left join sispacto3.identusutipodocumento d on d.iusd = i.iusd 
left join sispacto3.identificaoendereco e on e.iusd = i.iusd 
where (d.itdid is null or e.ienid is null) and iustermocompromisso=true
)";

$db->executar($sql);
$db->commit();


/*

// ATUALIZANDO AS CARACTERISTICAS DAS TURMAS NA QUAL O ORIENTADOR DE ESTUDOS VIRA FORMADOR

$sql = "UPDATE sispacto3.turmas x set uncid=foo.uncid, turdesc=foo.turdesc FROM (

select i.uncid, t.turid, replace(turdesc,'Turma OE','Turma FR') as turdesc FROM sispacto3.turmas t
inner join sispacto3.identificacaousuario i on i.iusd = t.iusd
inner join sispacto3.tipoperfil tp on tp.iusd = i.iusd and tp.pflcod=1131
inner join seguranca.perfil pe on pe.pflcod = tp.pflcod
where t.uncid is null

) foo where x.turid=foo.turid";

$db->executar($sql);
$db->commit();



// ATUALIZANDO O VINCULO COM A UNIVERSIDADE NO SIMEC COM BASE NA ABRANGENCIA DO MUNICIPIO (CL,OE E PA). MUITO COMUM QUANDO AS UNIVERSIDADES TROCAM MUNICIPIOS DE ABRANGENCIA NO DECORRER DO CURSO

$sql = "UPDATE sispacto3.usuarioresponsabilidade x set uncid=foo.uncidnovo FROM (

select i.iusd, i.iuscpf, e.uncid as uncidnovo FROM sispacto3.identificacaousuario i 
inner join sispacto3.tipoperfil t on t.iusd = i.iusd and t.pflcod in(1119,1120,1118)
inner join sispacto3.pactoidadecerta p on p.picid = i.picid 
inner join sispacto3.abrangencia a on a.muncod = p.muncod and a.esfera='M' 
inner join sispacto3.estruturacurso e on e.ecuid = a.ecuid 
where p.muncod is not null and i.uncid != e.uncid

) foo where x.usucpf = foo.iuscpf";

$db->executar($sql);
$db->commit();





$sql = "UPDATE sispacto3.usuarioresponsabilidade x SET uncid=xx.uncid FROM (

			select * FROM (
		
			select i.uncid, 
				   (select uncid FROM sispacto3.usuarioresponsabilidade where usucpf=i.iuscpf and pflcod=t.pflcod and uncid is not null and rpustatus='A' limit 1) as uncid2, 
				   i.iuscpf, 
				   t.pflcod 
			FROM sispacto3.identificacaousuario i
			inner join sispacto3.tipoperfil t on t.iusd = i.iusd
		
			) foo where foo.uncid!=foo.uncid2
		
			) xx where x.usucpf=xx.iuscpf and x.pflcod=xx.pflcod and rpustatus='A'";

$db->executar($sql);
$db->commit();






$sql = "UPDATE sispacto3.identificacaousuario set iustipoorientador='orientadorsispacto3013' where iusd in(

		select i.iusd FROM sispacto3.identificacaousuario i INNER JOIN sispacto3.tipoperfil t on t.iusd = i.iusd where t.pflcod=1120 and iustipoorientador!='orientadorsispacto3013' and i.iuscpf in(
		
		select i.iuscpf FROM sispacto.identificacaousuario i 
		inner join sispacto.tipoperfil t on t.iusd = i.iusd and t.pflcod=827 
		inner join sispacto.mensario m on m.iusd = i.iusd 
		inner join sispacto.mensarioavaliacoes ma on ma.menid = m.menid 
		where mavrecomendadocertificacao=1
		
		)
		
		)";

$db->executar($sql);
$db->commit();

$sql = "UPDATE sispacto3.identificacaousuario set iustipoorientador='professorsispacto3013' where iusd in(

		select i.iusd FROM sispacto3.identificacaousuario i INNER JOIN sispacto3.tipoperfil t on t.iusd = i.iusd where t.pflcod=1120 and iustipoorientador!='professorsispacto3013' and iustipoorientador!='orientadorsispacto3013' and i.iuscpf in(
		
		select i.iuscpf FROM sispacto.identificacaousuario i 
		inner join sispacto.tipoperfil t on t.iusd = i.iusd and t.pflcod=849 
		inner join sispacto.mensario m on m.iusd = i.iusd 
		inner join sispacto.mensarioavaliacoes ma on ma.menid = m.menid 
		where mavrecomendadocertificacao=1
		
		)
		)";

$db->executar($sql);
$db->commit();

$sql = "UPDATE sispacto3.identificacaousuario set picid=null where iusd in(
		
		select i.iusd FROM sispacto3.identificacaousuario i 
		inner join sispacto3.tipoperfil t on t.iusd = i.iusd 
		inner join seguranca.perfil p on p.pflcod = t.pflcod
		where t.pflcod not in(1119,1120,1118) and picid is not null
		
		)";

$db->executar($sql);
$db->commit();

$sql = "UPDATE sispacto3.identificacaousuario x set picid=foo.picid FROM (

		select i.iusd, i2.picid FROM sispacto3.identificacaousuario i 
		inner join sispacto3.tipoperfil t on t.iusd = i.iusd and t.pflcod=1118 
		left join sispacto3.orientadorturma ot on ot.iusd = i.iusd 
		inner join sispacto3.turmas tu on tu.turid = ot.turid 
		inner join sispacto3.identificacaousuario i2 on i2.iusd = tu.iusd 
		inner join sispacto3.tipoperfil t2 on t2.iusd = i2.iusd and t2.pflcod=1120 
		where i.picid != i2.picid
		
		) foo where x.iusd = foo.iusd";

$db->executar($sql);
$db->commit();




$sql = "UPDATE sispacto3.identificacaousuario x set uncid=xx.ecu FROM (
		select i.uncid as ius, e.uncid as ecu, i.iusd FROM sispacto3.identificacaousuario i 
		inner join sispacto3.tipoperfil t on t.iusd = i.iusd
		inner join sispacto3.pactoidadecerta p on p.picid = i.picid 
		inner join sispacto3.abrangencia a on a.muncod = p.muncod and esfera='M'
		inner join sispacto3.estruturacurso e on e.ecuid = a.ecuid 
		inner join sispacto3.universidadecadastro u on u.uncid = e.uncid 
		inner join workflow.documento d on d.docid = u.docid 
		where (i.uncid is null or i.uncid != e.uncid) and d.esdid=993
		) xx where xx.iusd = x.iusd";

$db->executar($sql);
$db->commit();


$sql = "UPDATE sispacto3.identificacaousuario x set uncid=xx.ecu FROM (
	
		select i.uncid as ius, e.uncid as ecu, i.iusd FROM sispacto3.identificacaousuario i 
		inner join sispacto3.tipoperfil t on t.iusd = i.iusd
		inner join sispacto3.pactoidadecerta p on p.picid = i.picid and p.estuf is not null
		inner join territorios.municipio m on m.muncod = i.muncodatuacao 
		inner join sispacto3.abrangencia a on a.muncod = m.muncod and esfera='E'
		inner join sispacto3.estruturacurso e on e.ecuid = a.ecuid 
		inner join sispacto3.universidadecadastro u on u.uncid = e.uncid 
		inner join workflow.documento d on d.docid = u.docid 
		where i.uncid is null and d.esdid=993
		
		) xx where xx.iusd = x.iusd";


$db->executar($sql);
$db->commit();

$sql = "UPDATE sispacto3.identificacaousuario x set uncid=foo.uncid FROM (
			select i.uncid, i2.iusd FROM sispacto3.identificacaousuario i
			inner join sispacto3.pactoidadecerta p on i.picid = p.picid 
			inner join sispacto3.tipoperfil t1 on t1.iusd = i.iusd and t1.pflcod=1120
			inner join sispacto3.turmas t on t.iusd = i.iusd 
			inner join sispacto3.orientadorturma ot on ot.turid = t.turid 
			inner join sispacto3.identificacaousuario i2 on i2.iusd = ot.iusd 
			inner join sispacto3.tipoperfil t2 on t2.iusd = i2.iusd and t2.pflcod=1118
			where i.uncid!=i2.uncid
			) foo where x.iusd = foo.iusd";

$db->executar($sql);
$db->commit();

$sql = "DELETE FROM sispacto3.impressoesana WHERE imaid IN(
		SELECT imaid FROM sispacto3.impressoesana i 
		INNER JOIN sispacto3.turmasprofessoresalfabetizadores pa ON pa.tpaid = i.tpaid 
		WHERE pa.tpastatus='I'
		)";

$db->executar($sql);
$db->commit();


$sql = "UPDATE sispacto3.mensarioavaliacoes ma SET mavtotal=foo.total FROM (
			select * FROM (
			select
			mavid,
			mavfrequencia,
			mavatividadesrealizadas,
			mavmonitoramento,
			mavtotal,
			(coalesce((mavfrequencia*fatfrequencia),0) + coalesce((mavatividadesrealizadas*fatatividadesrealizadas),0) + coalesce(mavmonitoramento,0)) as total
			FROM sispacto3.mensarioavaliacoes ma
			inner join sispacto3.mensario m ON m.menid = ma.menid
			inner join sispacto3.identificacaousuario u ON u.iusd = m.iusd
			inner join sispacto3.tipoperfil t ON t.iusd = u.iusd
			inner join sispacto3.fatoresdeavaliacao f ON f.fatpflcodavaliado = t.pflcod
			) fee
			where fee.mavtotal != total
			) foo
			where ma.mavid = foo.mavid";

$db->executar($sql);
$db->commit();


$sql = "UPDATE sispacto3.mensarioavaliacoes x set mavmonitoramento=foo.fatmonitoramento FROM (

			select mm.*, f.*, d.esdid FROM sispacto3.mensario m
			inner join sispacto3.tipoperfil t on t.iusd = m.iusd and t.pflcod!=".PFL_PROFESSORALFABETIZADOR."
			INNER JOIN sispacto3.fatoresdeavaliacao f ON f.fatpflcodavaliado = t.pflcod
			inner join workflow.documento d on d.docid = m.docid and d.esdid in(".ESD_APROVADO_MENSARIO.",".ESD_ENVIADO_MENSARIO.")
			inner join sispacto3.mensarioavaliacoes mm on mm.menid = m.menid
			where mavmonitoramento=0
		
			) foo where foo.mavid = x.mavid";

$db->executar($sql);
$db->commit();


$sql = "UPDATE sispacto3.mensario x set pflcod=(select pflcod FROM sispacto3.tipoperfil where iusd=foo.iusd) FROM (
		select iusd, menid FROM sispacto3.mensario where pflcod is null
		) foo
		where x.menid = foo.menid";

$db->executar($sql);
$db->commit();

$sql = "UPDATE sispacto3.mensarioavaliacoes x set pflcodavaliador=(select pflcod FROM sispacto3.tipoperfil where iusd=foo.iusdavaliador) from (
		select mavid, iusdavaliador from sispacto3.mensarioavaliacoes where pflcodavaliador is null
		) foo
		where x.mavid = foo.mavid";

$db->executar($sql);
$db->commit();


// REMOVENDO OS ARQUIVOS TEMPORARIOS

$dir    = DIRFILES . 'sispacto3/files_tmp';
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


$sql = "DELETE from sispacto3.orientadorturma where otuid in(
select ot.otuid from sispacto3.orientadorturma ot
inner join sispacto3.turmas tu ON tu.turid = ot.turid
inner join sispacto3.identificacaousuario i ON i.iusd = tu.iusd
inner join sispacto3.identificacaousuario i2 ON i2.iusd = ot.iusd
inner join sispacto3.pactoidadecerta p ON p.picid = i2.picid and p.muncod is not null
where i.uncid != i2.uncid
)";

$db->executar($sql);
$db->commit();

$sql = "DELETE from sispacto3.orientadorturmaoutros where otuid in(
select ot.otuid from sispacto3.orientadorturmaoutros ot
inner join sispacto3.turmas tu ON tu.turid = ot.turid
inner join sispacto3.identificacaousuario i ON i.iusd = tu.iusd
inner join sispacto3.identificacaousuario i2 ON i2.iusd = ot.iusd
inner join sispacto3.pactoidadecerta p ON p.picid = i2.picid and p.muncod is not null
where i.uncid != i2.uncid
)";

$db->executar($sql);
$db->commit();

$sql = "DELETE from sispacto3.orientadorturmaoutros where otuid in (
select otuid from sispacto3.orientadorturmaoutros ot
inner join sispacto3.turmas tu ON tu.turid = ot.turid
inner join sispacto3.identificacaousuario i ON i.iusd = tu.iusd
left join sispacto3.tipoperfil t ON t.iusd = i.iusd
where t.tpeid is null
)";

$db->executar($sql);
$db->commit();

$sql = "DELETE from sispacto3.historicoreaberturanota where mavid in (

	select ma.mavid from sispacto3.mensarioavaliacoes ma
	inner join sispacto3.mensario m ON m.menid = ma.menid
	inner join sispacto3.orientadorturma o ON o.iusd = m.iusd
	inner join sispacto3.turmas t ON t.turid = o.turid
	where m.pflcod=1118 and t.iusd!=ma.iusdavaliador and ma.menid in(
	select foo.menid from (
	select m.menid, (select count(*) from sispacto3.mensarioavaliacoes where menid=m.menid) as tt from sispacto3.mensario m
	inner join sispacto3.tipoperfil t ON t.iusd = m.iusd
	inner join workflow.documento d ON d.docid = m.docid
	where t.pflcod=1118 and d.esdid!=989
	) foo where foo.tt > 1
	)

	)";

$db->executar($sql);
$db->commit();


$sql = "DELETE FROM sispacto3.mensarioavaliacoes WHERE mavid IN (

	SELECT ma.mavid FROM sispacto3.mensarioavaliacoes ma
	INNER JOIN sispacto3.mensario m ON m.menid = ma.menid
	INNER JOIN sispacto3.orientadorturma o ON o.iusd = m.iusd
	INNER JOIN sispacto3.turmas t ON t.turid = o.turid
	WHERE m.pflcod=".PFL_PROFESSORALFABETIZADOR." AND t.iusd!=ma.iusdavaliador AND ma.menid IN(
		
	SELECT foo.menid FROM (
		
	SELECT m.menid, (SELECT count(*) FROM sispacto3.mensarioavaliacoes WHERE menid=m.menid) as tt FROM sispacto3.mensario m
	INNER JOIN sispacto3.tipoperfil t ON t.iusd = m.iusd
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
		select iuscpf, pf.pflcod from sispacto3.identificacaousuario i
		inner join seguranca.perfilusuario pp ON pp.usucpf = i.iuscpf
		inner join seguranca.perfil p ON p.pflcod = pp.pflcod and p.sisid=181
		inner join sispacto3.pagamentoperfil pf ON pf.pflcod = p.pflcod
		inner join sispacto3.tipoperfil t ON t.iusd = i.iusd
		where i.iusstatus='A' and pf.pflcod!=t.pflcod
		) foo WHERE p.usucpf=foo.iuscpf AND foo.pflcod=p.pflcod";

$db->executar($sql);
$db->commit();


// CRIANDO TURMAS DOS FORMADORES QUE SÃO INCLUIDOS DEPOIS DO PROJETO VALIDADO

$sql = "INSERT INTO sispacto3.turmas(
uncid, iusd, turdesc, turstatus, picid, muncod)

select i.uncid, i.iusd, 'TURMA FR - '||i.iusnome as turma, 'A', null, null from sispacto3.identificacaousuario i 
inner join sispacto3.tipoperfil t ON t.iusd = i.iusd 
inner join sispacto3.universidadecadastro u ON u.uncid = i.uncid 
inner join workflow.documento d ON d.docid = u.docid 
left join sispacto3.turmas tu ON tu.iusd = i.iusd 
where i.iusstatus='A' AND t.pflcod=1131 and d.esdid=993 and tu.turid is null";

$db->executar($sql);
$db->commit();


$sql = "UPDATE sispacto3.identificacaousuario f set iustipoprofessor=foo.tipop from (

	select i.iusd, i.iuscpf, i.iustipoprofessor, case when p.cpf is null then 'cpflivre' else 'censo' end as tipop from sispacto3.identificacaousuario i
	inner join sispacto3.tipoperfil t ON t.iusd = i.iusd and pflcod=1118
	left join sispacto3.professoresalfabetizadores p ON p.cpf = i.iuscpf
	where (i.iustipoprofessor != case when p.cpf is null then 'cpflivre' else 'censo' end or i.iustipoprofessor is null) and i.iusstatus='A'

	) foo where foo.iusd = f.iusd and foo.tipop='censo'";

$db->executar($sql);
$db->commit();


$sql = "DELETE FROM sispacto3.certificacao";
$db->executar($sql);
$db->commit();

$sql = "INSERT INTO sispacto3.certificacao(
		iusd, pflcod, cerfrequencia)
		select foo.iusd, foo.pflcod, case when foo.freq > 100 then 100.0 else foo.freq end as cerfrequencia from (
		select i.iusd, round((sum(ma.mavfrequencia)/(select count(*) from sispacto3.folhapagamentouniversidade where pflcod=m.pflcod and uncid=i.uncid))*100,1) as freq, m.pflcod from sispacto3.mensario m
		inner join sispacto3.mensarioavaliacoes ma on ma.menid = m.menid and ma.pflcodavaliador=1131
		inner join sispacto3.identificacaousuario i on i.iusd = m.iusd
		where m.pflcod=1120 and i.uncid is not null
		group by i.iusd,m.pflcod,i.uncid
		) foo";

$db->executar($sql);
$db->commit();

$sql = "INSERT INTO sispacto3.certificacao(
		iusd, pflcod, cerfrequencia)
		select i.iusd, t.pflcod, '0.0' as freq from sispacto3.identificacaousuario i
		inner join sispacto3.tipoperfil t on t.iusd = i.iusd and t.pflcod=1120
		left join sispacto3.certificacao c on c.iusd = i.iusd
		where c.cerfrequencia is null and i.uncid is not null and i.iuscpf not ilike 'SIS%'";

$db->executar($sql);
$db->commit();

$sql = "INSERT INTO sispacto3.certificacao(
		iusd, pflcod, cerfrequencia)
		select foo.iusd, foo.pflcod, case when foo.freq > 100 then 100.0 else foo.freq end as cerfrequencia from (
		select i.iusd, round((sum(ma.mavfrequencia)/(select count(*) from sispacto3.folhapagamentouniversidade where pflcod=m.pflcod and uncid=i.uncid))*100,1) as freq, m.pflcod from sispacto3.mensario m
		inner join sispacto3.mensarioavaliacoes ma on ma.menid = m.menid and ma.pflcodavaliador=1120
		inner join sispacto3.identificacaousuario i on i.iusd = m.iusd
		where m.pflcod=1118 and i.uncid is not null
		group by i.iusd,m.pflcod,i.uncid
		) foo";

$db->executar($sql);
$db->commit();

$sql = "INSERT INTO sispacto3.certificacao(
		iusd, pflcod, cerfrequencia)
		select i.iusd, t.pflcod, 0.0 as freq from sispacto3.identificacaousuario i
		inner join sispacto3.tipoperfil t on t.iusd = i.iusd and t.pflcod=1118
		left join sispacto3.certificacao c on c.iusd = i.iusd
		where i.iusd is null and i.uncid is not null and i.iuscpf not ilike 'SIS%'";

$db->executar($sql);
$db->commit();

$sql = "UPDATE sispacto3.certificacao SET cerfrequencia=0.0 WHERE cerfrequencia IS NULL";
$db->executar($sql);
$db->commit();
*/

$sql = "UPDATE seguranca.agendamentoscripts SET agstempoexecucao='".round((getmicrotime() - $microtime),2)."' WHERE agsfile='sispacto3_scripts_manutencao.php'";
$db->executar($sql);
$db->commit();


$db->close();


?>