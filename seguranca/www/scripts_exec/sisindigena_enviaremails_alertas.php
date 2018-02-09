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
require_once APPRAIZ . "www/sisindigena/_constantes.php";
require_once APPRAIZ . "www/sisindigena/_funcoes.php";

require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';


// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';


   
// abre conexção com o servidor de banco de dados
$db = new cls_banco();

$sql = "select
	   i.iuscpf,
	   i.iusnome as nome, 
       i.iusemailprincipal as email, 
       us.ususenha as senha
from sisindigena.identificacaousuario i 
inner join sisindigena.tipoperfil t on t.iusd = i.iusd 
inner join sisindigena.nucleouniversidade nu on nu.picid = i.picid  
inner join sisindigena.universidadecadastro un on un.uncid = nu.uncid 
inner join workflow.documento d on d.docid = un.docid 
inner join workflow.estadodocumento e on e.esdid = d.esdid
inner join workflow.documento d2 on d2.docid = nu.docid 
inner join workflow.estadodocumento e2 on e2.esdid = d2.esdid
left join seguranca.usuario us ON us.usucpf = i.iuscpf
where t.pflcod=1032 and e.esdid=858 and e2.esdid=858";

$foos = $db->carregar($sql);

if($foos[0]) {
	foreach($foos as $foo) {
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - Saberes Indígenas na Escola - Supervisor da IES";
		
		$mensagem->AddAddress( $foo['email'], $foo['nome'] );
		
			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']} (Supervisor da IES),</p>
						   <p>Você esta cadastrado como Supervisor da IES do programa Saberes Indígenas na Escola - SIMEC.</p>
						   <p>Inicialmente precisamos que você acesse o sistema e preencha alguns passos:</p>
						   <p><b>Passo 1 :</b> Aba \"Supervisor IES\". Preencher todos os dados solicitados. Esses dados serão enviados para o FNDE e serão utilizadas para o pagamento da bolsa.</p>
						   <p><b>Passo 2 :</b> Aba \"Definir Equipe\". Será feito o cadastro dos perfis ligados ao seu perfil (Orientadores de Estudo e Professores Alfabetizadores), esse cadastro será analisado pelo Coordenador Geral da Sede e pelo MEC, podendo ser modificado.</p>
						   <p><b>Passo 3 :</b> Aba \"Visualização do Plano de Trabalho\". Confirmar os dados inseridos e clicar no link \"Enviar para análise do MEC\". É muito importante o clique no link para confirmar a finalização do cadastramento das informações.</p>
						   <p>Att.<br>Equipe Saberes Indígenas</p>
						   <p>[ASSIM QUE FOR PREENCHIDO, ESTE E-MAIL NÃO SERÁ MAIS ENVIADO]</p>
		
		
		 <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"Não cadastrada")."</p>
		 ";
		
		
		$mensagem->IsHTML( true );
		
		if(!strstr($_SERVER['HTTP_HOST'],"simec-local")){
			$resp = $mensagem->Send();
			echo "sisindigena Supervisor IES _ ".$foo['nome']." - ".$foo['email']." : ".$resp;
		}
		
	}
}


$sql = "select i.iusnome as nome, 
       i.iusemailprincipal as email, 
       us.ususenha as senha
from sisindigena.identificacaousuario i 
inner join sisindigena.tipoperfil t on t.iusd = i.iusd 
inner join sisindigena.universidadecadastro un on un.uncid = i.uncid 
inner join workflow.documento d on d.docid = un.docid 
inner join workflow.estadodocumento e on e.esdid = d.esdid
left join seguranca.usuario us ON us.usucpf = i.iuscpf
where t.pflcod=1030 and d.esdid!=859";

$foos = $db->carregar($sql);

if($foos[0]) {
	foreach($foos as $foo) {
		
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - Saberes Indígenas na Escola - Coordenador Geral";
		
		$mensagem->AddAddress( $foo['email'], $foo['nome'] );
		
			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']} (Coordenador geral),</p>
						   <p>Você esta cadastrado como Coordenador Geral da Sede do programa Saberes Indígenas na Escola - SIMEC.</p>
						   <p>Inicialmente precisamos que você acesse o sistema e preencha alguns passos:</p>
						   <p><b>Passo 1 :</b> Aba \"Coordenador IES\". Preencher todos os dados solicitados. Esses dados serão enviados para o FNDE e serão utilizadas para o pagamento da bolsa.</p>
						   <p><b>Passo 2 :</b> Aba \"Orçamento\". Informar os valores referentes ao orçamento utilizado pela universidade no programa Saberes Indígenas na Escola (não incluir os gastos com as bolsas de estudos ). Tentar detalhar ao máximo os valores.</p>
						   <p><b>Passo 3 :</b> Aba \"Núcleos\". Foi feito um pré-cadastro dos núcleos e dos coordenadores de cada núcleo, caso tenha alguma modificação, a ferramenta permite inserir/remover núcleos e alterar o coordenador deste núcleo (lembrando que todas essas informações serão validadas pelo MEC).</p>
						   <p><b>Passo 4 :</b> Aba \"Visualização do Plano de Trabalho\". Confirmar os dados inseridos e clicar no link \"Enviar para análise do MEC\". É muito importante o clique no link para confirmar a finalização do cadastramento das informações.</p>
						   <p>Att.<br>Equipe Saberes Indígenas</p>
						   <p>[ASSIM QUE FOR PREENCHIDO, ESTE E-MAIL NÃO SERÁ MAIS ENVIADO]</p>
		
		
		 <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"Não cadastrada")."</p>
		 ";
		
		
		$mensagem->IsHTML( true );
		
		if(!strstr($_SERVER['HTTP_HOST'],"simec-local")){
			$resp = $mensagem->Send();
			echo "sisindigena Coordenador geral IES _ ".$foo['nome']." - ".$foo['email']." : ".$resp;
		}
	}
}



$sql = "select i.iusnome as nome, 
       i.iusemailprincipal as email, 
       us.ususenha as senha
from sisindigena.identificacaousuario i 
inner join sisindigena.tipoperfil t on t.iusd = i.iusd 
inner join sisindigena.nucleouniversidade p on p.picid = i.picid 
inner join sisindigena.universidadecadastro u on u.uncid = p.uncid 
inner join workflow.documento d on d.docid = u.docid 
inner join workflow.estadodocumento e on e.esdid = d.esdid
left join seguranca.usuario us ON us.usucpf = i.iuscpf
where t.pflcod=1031 and d.esdid!=860";

$foos = $db->carregar($sql);

if($foos[0]) {
	foreach($foos as $foo) {
		
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - Saberes Indígenas na Escola - Coordenador Adjunto";
		
		$mensagem->AddAddress( $foo['email'], $foo['nome'] );
		
			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']} (Coordenador Adjunto - Núcleo),</p>
						   <p>Você esta cadastrado como Coordenador Adjunto do programa Saberes Indígenas na Escola - SIMEC.</p>
						   <p>Inicialmente precisamos que você acesse o sistema e preencha alguns passos:</p>
						   <p><b>Passo 1 :</b> Aba \"Dados Coordenador Adjunto\". Preencher todos os dados solicitados. Esses dados serão enviados para o FNDE e serão utilizadas para o pagamento da bolsa.</p>
						   <p><b>Passo 2 :</b> Aba \"Projeto Pedagógico\". Informar Territórios, Povos, Aldeias, Línguas, etc e responder alguma perguntas.</p>
						   <p><b>Passo 3 :</b> Aba \"Definir Equipe\". Será feito o cadastro dos perfis ligados ao núcleo, esse cadastro será analisado pelo Coordenador Geral da Sede e pelo MEC, podendo ser modificado.</p>
						   <p><b>Passo 4 :</b> Aba \"Visualização do Plano de Trabalho\". Confirmar os dados inseridos e clicar no link \"Enviar para análise do MEC\". É muito importante o clique no link para confirmar a finalização do cadastramento das informações.</p>
						   <p>Att.<br>Equipe Saberes Indígenas</p>
						   <p>[ASSIM QUE FOR PREENCHIDO, ESTE E-MAIL NÃO SERÁ MAIS ENVIADO]</p>
		
		
		 <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"Não cadastrada")."</p>
		 ";
		
		
		$mensagem->IsHTML( true );
		
		if(!strstr($_SERVER['HTTP_HOST'],"simec-local")){
			$resp = $mensagem->Send();
			echo "sisindigena Coordenador adjunto IES _ ".$foo['nome']." - ".$foo['email']." : ".$resp;
		}
	}
}

// ENVIANDO EMAIL PARA TODOS QUE NÃO PREENCHERAM OS DADOS

$sql = "select i.iusnome as nome, i.iusemailprincipal as email, uu.ususenha as senha, pfl.pfldsc as perfil from sisindigena.identificacaousuario i
		inner join sisindigena.tipoperfil t on t.iusd = i.iusd 
		inner join seguranca.perfil pfl on pfl.pflcod = t.pflcod 
		inner join seguranca.perfilusuario pp on pp.pflcod = t.pflcod and i.iuscpf = pp.usucpf
		inner join seguranca.usuario_sistema us on us.usucpf=i.iuscpf and us.sisid=".SIS_INDIGENA." and us.suscod='A'
		inner join seguranca.usuario uu on uu.usucpf = i.iuscpf and uu.suscod='A'
		where i.iusstatus='A' AND i.iustermocompromisso is null";

$foos = $db->carregar($sql);

if($foos[0]) {
	foreach($foos as $foo) {

		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;

		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISIndígena - Preenchimento das dados cadastrais";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );

			
		$mensagem->Body = "<p>Prezado(a) {$foo['perfil']} - {$foo['nome']},</p>
		<p>Informamos que seu cadastro ja esta liberado no SIMEC, e é obrigatório o preenchimento dos dados para o recebimento da bolsa.</p>
		<p>Alem do preenchimento dos dados cadastrais (Aba de Dados {$foo['perfil']}).</p>
		<p>Att.<br/Equipe Saberes Indígenas</p>
		<br/><br/>
		<p>[ASSIM QUE FOR PREENCHIDO, ESTE E-MAIL NÃO SERÁ MAIS ENVIADO]</p>
		<p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"Não cadastrada")."</p>
				";

				$mensagem->IsHTML( true );
				$resp = $mensagem->Send();
				echo "Preenchimentos dos dados cadastrais _ ".$foo['nome']." - ".$foo['email']." : ".$resp;
	}
}




// ENVIANDO EMAIL PARA TODOS QUE NÃO REALIZARAM AVALIAÇÕES

$sql = "SELECT foo.iusnome as nome, foo.iusemailprincipal as email, foo.ususenha as senha, foo.referencia, foo.pflcod FROM (
SELECT distinct i.iusnome, t.pflcod, i.iusemailprincipal, usu.ususenha, rf.rfuparcela ||'º Parcela ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as referencia, CASE WHEN (dd.esdid NOT IN(919,925) OR dd.esdid IS NULL) THEN 'NOK' ELSE 'OK' END as ap
FROM sisindigena.folhapagamento f
INNER JOIN sisindigena.folhapagamentouniversidade rf ON rf.fpbid = f.fpbid
INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
INNER JOIN sisindigena.identificacaousuario i ON i.uncid = rf.uncid AND rf.picid = i.picid
INNER JOIN sisindigena.tipoperfil t ON t.iusd = i.iusd  AND t.pflcod IN(1030,1031,1032)
INNER JOIN seguranca.usuario_sistema us ON us.usucpf = i.iuscpf AND us.suscod='A' AND us.sisid=166
INNER JOIN seguranca.perfilusuario pu ON pu.usucpf = i.iuscpf AND pu.pflcod = t.pflcod
INNER JOIN seguranca.usuario usu ON usu.usucpf = i.iuscpf
LEFT JOIN sisindigena.mensario mm ON mm.iusd = i.iusd AND mm.fpbid = f.fpbid
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
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISIndígena - Avaliação da equipe";
		
		$mensagem->AddAddress( $foo['email'], $foo['nome'] );
		
			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		 <p>Informamos que seu cadastro ja esta liberado no SIMEC, e é fundamental que você faça avaliações sobre membros do projeto SISIndígena. Verificamos que você não fez a avaliação do período de referência: <b>".$foo['referencia']."</b></p>
		 <p>Para fazer a avaliação, acesse a aba de Execução e clique em Avaliar Equipe. Em seguida selecione as opções referentes a Frequência (caso seja obrigatório), Atividades Realizadas (caso seja obrigatório) e aperte o botão 'Salvar'.</p>
		 <p>Em seguida no ícone 'Enviar para análise'. Este passo é muito importante para a nota de monitoramento (parte da nota total da avaliação).</p>
		 <p>Equipe SISIndígena<br/>Ministério da Educação</p>
		 <br/><br/>
		 <p>ATENÇÃO – 'Saberes Indígenas na Escola - SISIndígena</p>
		 <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha ".(($foo['senha'])?md5_decrypt_senha( $foo['senha'], '' ):"Não cadastrada")."</p>
		 ";
		
		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Avaliação Geral _ ".$foo['nome']." - ".$foo['email']." : ".$resp;
	}
}


$sql = "select distinct i.iusnome as nome, i.iusemailprincipal as email from sisindigena.pagamentobolsista p 
		inner join workflow.documento d on d.docid = p.docid 
		inner join sisindigena.universidadecadastro u on u.uniid = p.uniid 
		inner join sisindigena.identificacaousuario i on i.uncid = u.uncid 
		inner join sisindigena.tipoperfil t on t.iusd = i.iusd 
		where t.pflcod=1030 and d.esdid=945";

$foos = $db->carregar($sql);

if($foos[0]) {
	foreach($foos as $foo) {

		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;

		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= SIGLA_SISTEMA;
		$mensagem->From 		= "noreply@mec.gov.br";
		$mensagem->Subject 		= SIGLA_SISTEMA. " - SISIndígena - Autorização de pagamentos";

		$mensagem->AddAddress( $foo['email'], $foo['nome'] );

			
		$mensagem->Body = "<p>Prezado(a) {$foo['nome']},</p>
		<p>Verificamos que você ja aprovou as avaliações pendentes dentro da sua equipe, porém ainda existem pagamentos que precisam ser encaminhados.</p>
		 <p>Para encaminhar os pagamentos, basta acessar \"Principal => Pagamentos\", selecione o período de referência e perfil, clique em listar pagamentos. Na lista de bolsistas, selecione os bolsistas que devem ser pagos e clique em Autorizar. Com isso dará início ao processo de pagamento.</p>
		 <p>Equipe SISIndígena<br/>Ministério da Educação</p>
		 <br/><br/>
		 <p>ATENÇÃO – 'Saberes Indígenas na Escola - SISIndígena</p>
		 <p>Para acessar o ambiente acesse http://simec.mec.gov.br, digite seu CPF e sua senha</p>
		 ";

		$mensagem->IsHTML( true );
		$resp = $mensagem->Send();
		echo "Autorização _ ".$foo['nome']." - ".$foo['email']." : ".$resp;
	}
}

$db->close();

?>