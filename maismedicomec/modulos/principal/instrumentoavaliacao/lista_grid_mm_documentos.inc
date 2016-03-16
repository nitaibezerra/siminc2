<?php
    #VERIFICA SE VARIÁVEIS DE SESSÃO FOI INICIADA. cASO NÃO, REDIRECIONA PARA PAGINA INICIAL.
    verificaExisteSessaoMM();
    
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

    if($_REQUEST['requisicao']) {
        $_REQUEST['requisicao']($_REQUEST, $_FILES);
    }

    include APPRAIZ . "includes/cabecalho.inc";
    echo "<br>";
    monta_titulo('Instrumento de Avaliação',"Serviços de saúde do município, {$_SESSION['maismedicomec']['mundescricao']}. <br> Candidato a sediar curso de medicina em IES privada." );
    echo "<br>";

    $abacod_tela    = ABA_QUESTIONARIO_MEDICINA;
    $url            = 'maismedicomec.php?modulo=principal/instrumentoavaliacao/lista_grid_mm_documentos&acao=A';
    $parametros     = '';

    $db->cria_aba($abacod_tela, $url, $parametros);
?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script type="text/javascript" language="javascript">
    
    function dowloadDocAnexoMaisMedicoPAR( arqid ){
        $('#arqid').val(arqid);
        $('#requisicao').val('dowloadDocAnexoMaisMedicoPAR');
        $('#formulario').submit();
    }
    
</script>

<form action="" method="POST" id="formulario" name="formulario" enctype="multipart/form-data">
    <input type="hidden" id="requisicao" name="requisicao" value=""/>
    <input type="hidden" id="arqid" name="arqid" value=""/>
        <div id="div_documento">
            <?PHP 
                $acao = "
                    <img border=\"0\" title=\"Baixar arquivo\" onclick=\"dowloadDocAnexoMaisMedicoPAR(\''|| am.arqid ||'\');\" style=\"cursor: pointer\" align=\"absmiddle\" src=\"../imagens/clipe1.gif\" />
                ";

                $sql = "
                    SELECT  '{$acao}' AS acao,
                            ar.arqnome||'.'||ar.arqextensao AS nome_arquivo,
                            ar.arqdescricao,
                            ta.tpadsc,
                            us.usunome
                    FROM par.arquivosmunicipio am
                    JOIN par.respquestaomaismedico rm ON am.rqmid = rm.rqmid
                    JOIN public.arquivo ar ON ar.arqid = am.arqid
                    JOIN par.tipoarquivo ta ON ta.tpaid = am.tpaid
                    JOIN seguranca.usuario us ON us.usucpf = ar.usucpf

                    WHERE am.aqmsituacao = 'A' AND rm.muncod = '{$_SESSION['maismedicomec']['muncod']}'

                    ORDER BY am.aqmdtinclusao DESC
                ";
                $alinhamento = array('center','left','left','left','left');
                $tamanho = array('2%','20%','20%','18%','14%');
                $cabecalho = array('Ação', 'Nome Arquivo','Descrição','Tipo de Arquivo','Responsável');
                $db->monta_lista($sql, $cabecalho, '50', '10', '', '', '', '',$tamanho, $alinhamento);
            ?>
        </div>
</form>

