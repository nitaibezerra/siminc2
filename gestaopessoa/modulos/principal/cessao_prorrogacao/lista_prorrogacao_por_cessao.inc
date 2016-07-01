<?php

    $cprid  = $_SESSION['gestao']['cprid'];

    if($cprid != ''){
        $docid = buscarDocidCessaoProrrogacao( $cprid );
        if($docid != ''){
            $estadoAtual = pegaEstadoAtualWorkflowCessao($docid);
        }
    }

    if( $cprid != '' ){
        $_SESSION['gestao']['cprid'] = $_GET['cprid'];
        $cprid = $_GET['cprid'];

        $dados = dadosServidor( $cprid );

        $nu_matricula_siape = trim( str_replace('-', '', str_replace( '.', '', $dados['nu_matricula_siape'] ) ) );

        $hab_cabecalho  = 'S';
    }


    include APPRAIZ . "includes/cabecalho.inc";
    echo "<br>";
    monta_titulo('Getão de Pessoas', '<b>CESSÃO - Cadastro de Portaria</b>');

    $abacod_tela    = ABA_CAD_CESSAO_SERVIDOR;
    $url            = 'gestaopessoa.php?modulo=principal/cessao_prorrogacao/lista_prorrogacao_por_cessao&acao=A';
    $parametros     = '&cprid='.$cprid;

    if($hab_cabecalho  == 'S'){
        cabecalhoAbas($cprid);
    }else{
        echo "<br>";
    }

    $db->cria_aba($abacod_tela, $url, $parametros);
?>

<script type="text/javascript">

    function editarProrrogacao(cprid){
        window.location.href ='gestaopessoa.php?modulo=principal/cessao_prorrogacao/cad_dados_prorrogacao&acao=A&cprid='+cprid;
    }

</script>

<?PHP
    $acao = "
        <center>
            <!--<img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"editarProrrogacao('||cp.cprid||');\" title=\"Editar Servidor\" >-->
            <img align=\"absmiddle\" src=\"/imagens/alterar_01.gif\">
        </center>
    ";

    $sql = "
        SELECT  '{$acao}' as acao,
                cp.cprid,
                oc.orcdsc, --Fluxo de origem
                cp.cprnumprocesso,
                CASE
                    WHEN cp.tpsid = 1 THEN '<img border=\"0\" align=\"absmiddle\" src=\"../imagens/icones/bb.png\" title=\"\" >'
                    WHEN cp.tpsid = 2 THEN '<img border=\"0\" align=\"absmiddle\" src=\"../imagens/icones/br.png\" title=\"\" >'
                    WHEN cp.tpsid = 3 THEN '<img border=\"0\" align=\"absmiddle\" src=\"../imagens/icones/bg.png\" title=\"\" >'
                END AS tpsid,

                replace(to_char(cast(s.nu_cpf as bigint), '000:000:000-00'), ':', '.') as nu_cpf,
                s.nu_matricula_siape AS nu_matricula_siape,
                s.no_servidor AS fdpnome,

                '' AS funcao_desem,
                '' AS cargo_ef,

                os.ogcdsc
        FROM gestaopessoa.cessaoprorrogacao cp

        JOIN siape.tb_servidor_simec s ON cast(s.nu_matricula_siape as character(7)) = cp.nu_matricula_siape AND co_situacao_servidor IN ( 1,8,11,43 )

        JOIN gestaopessoa.origemcessao oc ON oc.orcid = cp.orcid
        JOIN gestaopessoa.orgaocessionario os ON os.ogcid = cp.ogcid

        WHERE cp.cprtipo = 'P' AND cp.cprstatus = 'A' AND cp.nu_matricula_siape = '{$nu_matricula_siape}'

        ORDER BY 1
    ";
    $cabecalho = array("Ação", "Seq", "Origem Fluxo", "Nº do Porcesso", "Tipo", "CPF Servidor", "SIAPE", "Servidor", "Função Desempenhada", "Cargo Efetivo", "Órgão Cessionário");

    $align  = Array('left', 'left', 'left');
    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', '', $align, '');

?>

