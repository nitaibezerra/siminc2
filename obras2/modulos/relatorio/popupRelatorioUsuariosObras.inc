<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");
extract($_REQUEST);

$param = $_POST;
$where = array();

if ($param['estuf']) {
    $where[] = " u.regcod = '" . $param['estuf'] . "' ";
}

if ($param['muncod']) {
    $where[] = " m.muncod = '" . $param['muncod'] . "' ";
}

if ($param['suscod']) {
    $where[] = " us.suscod = '" . $param['suscod'] . "' ";
}

if ($param['obrid']) {
    $where[] = " " . $param['obrid'] . " IN (SELECT o.obrid FROM obras2.usuarioresponsabilidade ur
            JOIN obras2.obras o ON o.empid = ur.empid AND o.obridpai IS NULL AND o.obrstatus = 'A'
            WHERE ur.usucpf = u.usucpf AND ur.rpustatus = 'A') ";
}

if ($param['pflcod']) {
    $where[] = " " . $param['pflcod'] . " IN (SELECT DISTINCT p.pflcod  FROM obras2.usuarioresponsabilidade ur
                                                JOIN seguranca.perfil p ON ur.pflcod = p.pflcod
                                                WHERE ur.usucpf = u.usucpf AND p.pflstatus = 'A' AND ur.rpustatus = 'A') ";
}

if ($param['tpocod']) {
    $where[] = " u.tpocod = '" . $param['tpocod'] . "' ";
}

if ($param['entid']) {
    $where[] = " u.entid = '" . $param['entid'] . "' ";
}

if ($param['unicod']) {
    $where[] = " u.unicod = '" . $param['unicod'] . "' ";
}

if ($param['estuf']) {
    $where[] = " u.ungcod = '" . $param['ungcod'] . "' ";
}

$obras = "
        (array_to_string(
            array(
                SELECT o.obrid FROM obras2.usuarioresponsabilidade ur
                JOIN obras2.obras o ON o.empid = ur.empid AND o.obridpai IS NULL AND o.obrstatus = 'A'
                WHERE ur.usucpf = u.usucpf AND ur.rpustatus = 'A'), ', ')
        )
    ";
if ( $_REQUEST['pesquisa'] == 'xls' ) {
    $obras = "substr($obras, 0, 999)";
}

$sql = "
        SELECT
            u.regcod,
            m.mundescricao,
            u.usucpf,
            u.usunome,
            u.usuemail,
            '(' || usufoneddd || ')' || u.usufonenum as telefone,
            array_to_string(
                array(
                    SELECT DISTINCT p.pfldsc  FROM obras2.usuarioresponsabilidade ur
                    JOIN seguranca.perfil p ON ur.pflcod = p.pflcod
                    WHERE ur.usucpf = u.usucpf AND p.pflstatus = 'A' AND ur.rpustatus = 'A'
                ),', '
            ) AS perfil,
            CASE WHEN us.suscod = 'A' THEN
              'Ativo'
            WHEN us.suscod = 'B' THEN
              'Bloqueado'
            WHEN us.suscod = 'P' THEN
              'Pendente'
            ELSE
            '-' END as situacao,
            $obras as obras

        FROM seguranca.usuario u
            INNER JOIN seguranca.usuario_sistema us on us.usucpf = u.usucpf and sisid = 147 and us.susstatus = 'A'
            INNER JOIN territorios.municipio m ON m.muncod = u.muncod
            WHERE u.usucpf IN (
                                SELECT ur.usucpf FROM obras2.usuarioresponsabilidade ur
                                WHERE ur.pflcod IN (".PFLCOD_GESTOR_UNIDADE.", ".PFLCOD_SUPERVISOR_UNIDADE.") AND 
                                      ur.rpustatus = 'A'
                                group by ur.usucpf)
                  " . (count($where) ? " AND " . implode(' AND ', $where) : "") . "
        ORDER BY u.regcod, m.mundescricao";

$cabecalho = array("UF", "Município", "CPF", "Nome", "E-mail", "Telefone", "Perfil", "Situação");
// Gera o XLS do relatório
if ( $_REQUEST['pesquisa'] == 'xls' ){
    ini_set("memory_limit", "3024M");
    $dados = $db->carregar($sql);
    $coluns = 0;

    if(!empty($dados)) {
        foreach ($dados as $key => $usu) {
            $obras = explode(',', $usu['obras']);
            unset($dados[$key]['obras']);
            $coluns = ($coluns > count($obras)) ? $coluns : count($obras);
            $dados[$key] = $dados[$key] + $obras;
        }
        for ($x = 0; $x < $coluns; $x++)
            $cabecalho[] = 'Obra';
    } else {
        $dados = array();
    }
    $db->sql_to_xml_excel($dados, 'Relatorio_de_Usuarios_Obras', $cabecalho, '');
}

?>
<link rel="stylesheet" type="text/css" href="/includes/superTitle.css" />
<script type="text/javascript" src="/includes/remedial.js"></script>
<script type="text/javascript" src="/includes/superTitle.js"></script>
<html>
<head>
    <title> <?php echo NOME_SISTEMA; ?> </title>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
</head>
<body>
<center>
    <!--  Cabeçalho Brasão -->
    <?php echo monta_cabecalho_relatorio( '95' ); ?>
</center>
</body>
</html>
<?php
//    $dados = $db->carregar($sql);
$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', 'center', 'N', '', '','');
?>