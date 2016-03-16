<?php

if(!isset($_REQUEST['recuperar'])){
    echo 'Usuários inválidos!';
    die;
}

set_time_limit(0);

// carrega as funções gerais
require_once '../global/config.inc';
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '';
$_SESSION['usucpf'] = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<?
if($_REQUEST['limit']){
    // abre conexão com o servidor de banco de dados
    $db = new cls_banco();

    $sql = "select usucpf, usunome, usuemail, regcod, muncod, usufuncao, usufoneddd, usufonenum
            from seguranca.usuario
            order by usunome
            limit {$_REQUEST['limit']}
            offset {$_REQUEST['offset']}
            ";

    $dados = $db->carregar($sql);


    foreach ($dados as $dado):
        $aDadosUsuario = recuperarUsuarioReceita($dado['usucpf']);
        if($aDadosUsuario['usuarioexiste']){
            continue;
        }

        $sql = "insert into seguranca.cpfinvalido (usucpf) values ('{$dado['usucpf']}') returning cpiid";
        $cpiid = $db->pegaUm($sql);
        $db->commit();
        echo $cpiid . '<br />';

    endforeach;

    die;
}

$db = new cls_banco();

$sql = "select count(*)
            from seguranca.usuario
            ";
$total = $db->pegaUm($sql);

?>

<div id="lista-usuarios"></div>

<script type="text/javascript" src="../includes/JQuery/jquery-1.5.1.min.js"></script>
<script type="text/javascript">
    limit = 10000;
    offset = -10000;
    total = '<?php echo $total; ?>'
    $(function(){
        exibir();
    });

    function exibir()
    {
        offset = parseInt(offset) + parseInt(limit);

        if (total > offset) {
            $.ajax({
                url: 'listaCpfInvalido.php?recuperar=1&limit=' + limit + '&offset=' + offset,
                success: function($data) {
                    $('#lista-usuarios').append($data);
                    exibir();
                }
            });
        }
    }
</script>
