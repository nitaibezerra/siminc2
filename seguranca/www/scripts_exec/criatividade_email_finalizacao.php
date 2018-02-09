<?php
set_time_limit(0);

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );

$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento
// $_REQUEST['baselogin']  = "simec_desenvolvimento";//simec_desenvolvimento

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
// require_once "../../global/config.inc";

require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";

//eduardo - envio SMS pendecias de obras - PAR
//http://simec-local/seguranca/scripts_exec/par_enviaSMS_pendenciasAtualizacaoObras.php
// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
$_SESSION['sisid'] = 98;

$db = new cls_banco();

$sql = "select queid, quesituacao, parnome, parcpf, paremail, parsexo
        from criatividadeeducacao.questionario  q
            inner join criatividadeeducacao.participante p on p.parid = q.parid
        where coalesce(paremail, '') != ''
        and quesituacao = 'A'
        ";

$dados = $db->carregar($sql);
$dados = $dados ? $dados : array();

foreach ($dados as $dado) {

    $saudacao = $dado['parsexo'] == 'F' ? 'Prezada Sra.' : 'Prezado Sr.';

    $conteudo = "<pre>{$saudacao} {$dado['parnome']},

Percebemos que você iniciou sua inscrição na Chamada Pública Inovação e Criatividade na Educação Básica (http://siscriatividade.mec.gov.br), mas não a completou.
Alertamos que as três páginas do formulário devem ser preenchidas para que sua inscrição seja analisada. Ao final, clique em FINALIZAR.

Atenciosamente,

A Equipe do
Inovação e Criatividade na Educação Básica
</pre>";

    $assunto = "Inscrição na Chamada Pública Inovação e Criatividade na Educação Básica";

    $destinatario = trim($dado['paremail']);

    simec_email(array('nome'=>'Inovação e Criatividade na Educação Básica', 'email'=>$_SESSION['email_sistema']), $destinatario, $assunto, $conteudo);
}

echo 'FIM';