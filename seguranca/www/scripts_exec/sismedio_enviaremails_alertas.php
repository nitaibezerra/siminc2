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
require_once APPRAIZ . "www/sismedio/_constantes.php";
require_once APPRAIZ . "www/sismedio/_funcoes.php";

require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';


// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';

function getmicrotime() {list($usec, $sec) = explode(" ", microtime()); return ((float)$usec + (float)$sec);}

$microtime = getmicrotime();



   
// abre conexção com o servidor de banco de dados
$db = new cls_banco();

if(date("w")==2) {

	$sql = "select i.iusnome as nome, i.iusemailprincipal as email from sismedio.identificacaousuario i 
			inner join sismedio.tipoperfil t on t.iusd = i.iusd 
			where t.pflcod in(1076,1077) and i.uncid in(
			
			SELECT u.uncid FROM sismedio.universidadecadastro u
			INNER JOIN workflow.documento d ON d.docid = u.docid 
			INNER JOIN workflow.documento d2 ON d2.docid = u.docidturmaformadoresregionais 
			INNER JOIN workflow.documento d3 ON d3.docid = u.docidturmaorientadoresestudo
			WHERE d.esdid='931' AND d2.esdid=1200 AND d3.esdid=1200
			
			)";
	
	$foos = $db->carregar($sql);
	
	if($foos[0]) {
		foreach($foos as $foo) {
	
			$mensagem = new PHPMailer();
			$mensagem->persistencia = $db;
	
			$mensagem->Host         = "localhost";
			$mensagem->Mailer       = "smtp";
			$mensagem->FromName		= SIGLA_SISTEMA;
			$mensagem->From 		= "noreply@mec.gov.br";
			$mensagem->Subject 		= SIGLA_SISTEMA. " - SISMédio - LEMBRETE : Aprovar Equipe";
	
			$mensagem->AddAddress( $foo['email'], $foo['nome'] );
	
				
			$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
			<p>Uma vez por semana, a equipe do MEC vai enviar este e-mail para lembra-lo de aprovar a avaliação feita pelos membros da sua equipe. este procedimento irá garantir as bolsas de todos os participantes.</p>
			<p>Para aprovar a bolsa, basta acessar a aba Execução => Aprovar Equipe, selecione os períodos de refêrencia e os perfis, e clique no botão Aprovar. Essa atividade pode ser realizar pelo Coordenador Geral ou Adjuntos das universidades (é recomendado que estes façam essa atividade periodicamente).</p>
			<p>Secretaria de Educação Básica<br/>Ministério da Educação</p>
			<br/><br/>
			<p>ATENÇÃO – Pacto Nacional pelo Fortalecimento do Ensino Médio</p>
			<p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha</p>
			";
	
			$mensagem->IsHTML( true );
			$resp = $mensagem->Send();
			echo "Lembrete aprovar equipe _ ".$foo['nome']." - ".$foo['email']." : ".$resp."<br>";
		}
	}

}

$sql = "select i.iusd, i.iuscpf, i.iusnome as nome, i.iusemailprincipal as email, i.iusagenciasugerida, s.agencia, i.cadastradosgb
		from sismedio.bolsistaserroagencia s
		inner join sismedio.identificacaousuario i on i.iuscpf = trim(s.cpf)
		inner join sismedio.tipoperfil t on t.iusd = i.iusd
		where trim(i.iusagenciasugerida) = trim(s.agencia)";

$foos = $db->carregar($sql);

if($foos[0]) {
	foreach($foos as $foo) {

		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;

		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISMEDIO - Problemas com agência bancária";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );

			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		<p>Foi identificado problema com a agência selecionada no cadastro do SISMEDIO. Possivelmente sua agência foi invalidada ou desativada do programa.</p>
		<p>Pedimos que acesse o sistema e selecione outra agência bancária(na aba \"Dados\").</p>
		<p>Att.<br>Equipe do SISMEDIO</p>
		<p>[ASSIM QUE FOR ALTERADO, ESTE E-MAIL NÃO SERÁ MAIS ENVIADO]</p>


		<p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha</p>
		";

		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Agência Bancária ".$foo['nome']." - ".$foo['email']." : ".$resp;
	}
}


$sql = "SELECT i.iusnome as nome, i.iusemailprincipal as email FROM sismedio.identificacaousuario i 
		INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd AND t.pflcod IN(1076, 1078, 1077)
		WHERE i.uncid IN(
		
		SELECT u.uncid FROM sismedio.universidadecadastro u
		INNER JOIN workflow.documento d ON d.docid = u.docid 
		INNER JOIN workflow.documento d2 ON d2.docid = u.docidturmaformadoresregionais 
		INNER JOIN workflow.documento d3 ON d3.docid = u.docidturmaorientadoresestudo
		WHERE d.esdid='931' AND (d2.esdid!=1200 OR d3.esdid!=1200)
		
		)";

$foos = $db->carregar($sql);

if($foos[0]) {
	foreach($foos as $foo) {

		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;

		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISMédio -  Composição de turmas";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );

			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		<p>O projeto da sua universidade encontra-se validado pelo MEC, porém existem mais alguns passos para iniciarmos a execução do curso, e consequentemente o pagamento das bolsas.</p>
		<p>O primeiro passo é compor as turmas dos Formadores Regionais (Principal => Composição de turmas). Para efetuar este procedimento, basta acessar a turma do Formador e adicionar Orientadores de Estudos, e ao final clicar no link \"Concluir composição da turma\". Este passo pode ser efetuado pelo Coordenador Geral, Adjunto e Supervisores.</p>
		<p>O segundo passo é compor as turmas dos Orientadores de Estudo. Por padrão, o sistema faz um pré carregamento para as escolas que possuem apenas 1 orientador de estudo, porém os que possuem mais de 1, esses perfis devem aloca-los na devida turma. ao final clicar no link \"Concluir composição da turma\".</p>
		<p>Depois desses passos, iniciaremos a Execução do programa.</p>
		<p>Secretaria de Educação Básica<br/>Ministério da Educação</p>
		<br/><br/>
		<p>ATENÇÃO – Pacto Nacional pelo Fortalecimento do Ensino Médio</p>
		<p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha</p>
		";

		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Composição de turmas _ ".$foo['nome']." - ".$foo['email']." : ".$resp."<br>";
	}
}


/*
 * ALERTANDO TODOS OS PERFIS COM ACESSO AO SISPACTO DE PREENCHER O TERMO DE COMPROMISSO
*/

$sql = "select i.iusnome as nome, i.iusemailprincipal as email, u.ususenha as senha from sismedio.identificacaousuario i
		inner join sismedio.tipoperfil t on t.iusd = i.iusd
		inner join seguranca.perfilusuario pu on i.iuscpf = pu.usucpf and t.pflcod = pu.pflcod
		inner join seguranca.usuario_sistema us on us.usucpf = i.iuscpf and us.sisid=".SIS_MEDIO." AND us.suscod='A'
		inner join seguranca.usuario u on u.usucpf = i.iuscpf
		where i.iusstatus='A' and i.iustermocompromisso is null and i.uncid in(
		
				SELECT u.uncid FROM sismedio.universidadecadastro u
				INNER JOIN workflow.documento d ON d.docid = u.docid
				WHERE d.esdid='".ESD_VALIDADO_COORDENADOR_IES."'
		
		)";

$foos = $db->carregar($sql);

if($foos[0]) {
	foreach($foos as $foo) {
	
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISMédio -  Preenchimento dos dados cadastrais";
		
		$mensagem->AddAddress( $foo['email'], $foo['nome'] );
		
			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		<p>Informamos que seu acesso ja esta liberado no SIMEC. Solicitamos que acesse o sistema e preencha os dados solicitados para o recebimento da bolsa.</p>
		<p>Secretaria de Educação Básica<br/>Ministério da Educação</p>
		<br/><br/>
		<p>ATENÇÃO – Pacto Nacional pelo Fortalecimento do Ensino Médio</p>
		<p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"Não cadastrada")."</p>
		";
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Preenchimento das informações _ ".$foo['nome']." - ".$foo['email']." : ".$resp."<br>";
	}
}

$sql = "SELECT foo.iusnome as nome, foo.iusemailprincipal as email, foo.ususenha as senha, foo.referencia, foo.pfldsc FROM (
SELECT i.uncid, per.pfldsc, i.iusnome, i.iusemailprincipal, usu.ususenha, t.pflcod, rf.rfuparcela ||'º Parcela ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as referencia, CASE WHEN count(racid) > 0 THEN 'OK' ELSE 'NOK' END as ap
					FROM sismedio.folhapagamento f 
					INNER JOIN sismedio.folhapagamentouniversidade rf ON rf.fpbid = f.fpbid 
					INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia 
					INNER JOIN sismedio.identificacaousuario i ON i.uncid = rf.uncid 
					INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd  AND t.pflcod IN(1082,1088) AND rf.pflcod = t.pflcod
					INNER JOIN sismedio.pagamentoperfil pp ON pp.pflcod = t.pflcod 
					INNER JOIN seguranca.usuario_sistema us ON us.usucpf = i.iuscpf AND us.suscod='A' AND us.sisid=174 
					INNER JOIN seguranca.perfilusuario pu ON pu.usucpf = i.iuscpf AND pu.pflcod = t.pflcod 
					INNER JOIN seguranca.usuario usu ON usu.usucpf = i.iuscpf 
					INNER JOIN seguranca.perfil per ON per.pflcod = t.pflcod 
					LEFT JOIN sismedio.respostasavaliacaocomplementar mm ON mm.iusdavaliador = i.iusd AND mm.fpbid = f.fpbid
					WHERE f.fpbstatus='A' AND to_char(NOW(),'YYYYmmdd')>=to_char((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'15')::date,'YYYYmmdd')
GROUP BY i.uncid, per.pfldsc, i.iusnome, i.iusemailprincipal, usu.ususenha, t.pflcod, rf.rfuparcela, m.mesdsc, fpbanoreferencia 
) foo WHERE foo.ap='NOK'";

$foos = $db->carregar($sql);

if($foos[0]) {
	foreach($foos as $foo) {

		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;

		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISMédio - Avaliação complementar";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );
			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		<p>Informamos que seu cadastro ja esta liberado no SIMEC, e é obrigatório que você preencha o espaçõ de avaliação complementar. Verificamos que você não fez a avaliação do período de referência: <b>".$foo['referencia']."</b></p>
		 <p>Para fazer a avaliação, acesse o SIMEC e clique em Avaliação Complementar. Em seguida selecione as opções referentes e aperte o botão 'Salvar'.</p>
		 <p>Secretaria de Educação Básica<br/>Ministério da Educação</p>
		 <br/><br/>
		 <p>ATENÇÃO – Pacto Nacional pelo Fortalecimento do Ensino Médio</p>
		 <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"Não cadastrada")."</p>";

		 $mensagem->IsHTML( true );
		 $resp = $mensagem->Send();
		 echo "avaliação equipe _ ".$foo['nome']." - ".$foo['email']." : ".$resp."<br>";
	}
}

$sql = "select i.iusnome as nome, i.iusemailprincipal as email, usu.ususenha as senha, fu.rfuparcela ||'º Parcela ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as referencia from sismedio.identificacaousuario i 
inner join sismedio.tipoperfil t on t.iusd = i.iusd 
inner join sismedio.folhapagamentouniversidade fu on fu.pflcod = t.pflcod and fu.uncid = i.uncid 
inner join sismedio.folhapagamento f on f.fpbid = fu.fpbid 
inner join seguranca.usuario usu ON usu.usucpf = i.iuscpf
inner join public.meses m on m.mescod::integer = f.fpbmesreferencia
left join sismedio.cadernoatividadesrespostas ca on ca.iusd = i.iusd and fu.fpbid = ca.fpbid and ca.caroeproposatividadecadernoformacao is not null
where t.pflcod in(1082,1088) and i.iusstatus='A' and ca.carid is null";

$foos = $db->carregar($sql);

if($foos[0]) {
	foreach($foos as $foo) {

		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;

		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISMédio - Avaliação obrigatória";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );
			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		<p>O MEC possui interesse em conhecer o trabalho feito pelo professor no Pacto Nacional pelo Fortalecimento do Ensino Médio. E para isso criamos uma atividade na qual você deve informar as atividades realizadas. Verificamos que você ainda não fez essa atividade no período: <b>".$foo['referencia']."</b></p>
		 <p>Para fazer a atividade, acesse o SIMEC e clique em Avaliação Obrigatória. Em seguida clique nas opções disponíveis, insira as atividades e complete o formulário.</p>
		 <p>Secretaria de Educação Básica<br/>Ministério da Educação</p>
		 <br/><br/>
		 <p>ATENÇÃO – Pacto Nacional pelo Fortalecimento do Ensino Médio</p>
		 <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"Não cadastrada")."</p>";

		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "avaliação equipe _ ".$foo['nome']." - ".$foo['email']." : ".$resp."<br>";
	}
}


$sql = "SELECT foo.iusnome as nome, foo.iusemailprincipal as email, foo.ususenha as senha, foo.referencia, foo.pfldsc FROM (
	SELECT i.uncid, per.pfldsc, i.iusnome, i.iusemailprincipal, usu.ususenha, t.pflcod, rf.rfuparcela ||'º Parcela ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as referencia, CASE WHEN (dd.esdid NOT IN(951,957) OR dd.esdid IS NULL) THEN 'NOK' ELSE 'OK' END as ap
					FROM sismedio.folhapagamento f 
					INNER JOIN sismedio.folhapagamentouniversidade rf ON rf.fpbid = f.fpbid 
					INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia 
					INNER JOIN sismedio.identificacaousuario i ON i.uncid = rf.uncid 
					INNER JOIN sismedio.tipoperfil t ON t.iusd = i.iusd  AND t.pflcod IN(1076,1190,1078,1077,1081) AND rf.pflcod = t.pflcod
					INNER JOIN sismedio.pagamentoperfil pp ON pp.pflcod = t.pflcod 
					INNER JOIN seguranca.usuario_sistema us ON us.usucpf = i.iuscpf AND us.suscod='A' AND us.sisid=181 
					INNER JOIN seguranca.perfilusuario pu ON pu.usucpf = i.iuscpf AND pu.pflcod = t.pflcod 
					INNER JOIN seguranca.usuario usu ON usu.usucpf = i.iuscpf 
					INNER JOIN seguranca.perfil per ON per.pflcod = t.pflcod 
					LEFT JOIN sismedio.mensario mm ON mm.iusd = i.iusd AND mm.fpbid = f.fpbid and mm.pflcod = t.pflcod 
					LEFT JOIN workflow.documento dd ON dd.docid = mm.docid 
					WHERE f.fpbstatus='A' AND to_char(NOW(),'YYYYmmdd')>=to_char((fpbanoreferencia::text||lpad(fpbmesreferencia::text, 2, '0')||'15')::date,'YYYYmmdd')
		) foo WHERE foo.ap='NOK'";

$foos = $db->carregar($sql);

if($foos[0]) {
	foreach($foos as $foo) {

		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;

		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISMédio - Avaliação da equipe";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );
			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		 <p>Informamos que seu cadastro ja esta liberado no SIMEC, e é fundamental que você faça avaliações sobre a equipe do projeto Pacto Nacional pelo Fortalecimento do Ensino Médio. Verificamos que você não fez a avaliação do período de referência: <b>".$foo['referencia']."</b></p>
		 <p>Para fazer a avaliação, acesse a aba de Execução e clique em Avaliar Equipe. Em seguida selecione as opções referentes a Frequência (caso seja obrigatório), Atividades Realizadas (caso seja obrigatório) e aperte o botão 'Salvar'.</p>
		 <p>Em seguida no ícone 'Enviar para análise'. Este passo é muito importante para a nota de monitoramento (parte da nota total da avaliação).</p>
		 <p>Secretaria de Educação Básica<br/>Ministério da Educação</p>
		 <br/><br/>
		 <p>ATENÇÃO – Pacto Nacional pelo Fortalecimento do Ensino Médio</p>
		 <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"Não cadastrada")."</p>";

		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "avaliação equipe _ ".$foo['nome']." - ".$foo['email']." : ".$resp."<br>";
	}
}

$sql = "UPDATE seguranca.agendamentoscripts SET agstempoexecucao='".round((getmicrotime() - $microtime),2)."' WHERE agsfile='sismedio_enviaremails_alertas.php'";
$db->executar($sql);
$db->commit();


$db->close();

?>