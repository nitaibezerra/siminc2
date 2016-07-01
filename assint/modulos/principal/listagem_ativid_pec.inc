
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script type="text/javascript">

//#CARREGAR LISTA FILHO
    function exibirListaAtividades(pecid, td_nome){
	divCarregando();
	$.ajax({
            type    : "POST",
            //url     : window.location.href,
            url     : 'assint.php?modulo=principal/PEC_G&acao=A&aba=listagem_ativid_pec',
            data    : "requisicao=exibirListaAtividades&pecid="+pecid,
            async   : false,
            success: function(msg){
                $('#'+td_nome).html(msg);
                divCarregado();
            }
	});
    }

    //#CARREGAR LISTA FILHO
    function carregarAtividadesPEC(idImg, pecid){
        var img     = $( '#'+idImg );
        var tr_nome = 'listaAtidades_'+ pecid;
        var td_nome = 'trA_'+ pecid;

        if( $('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() == "" ){
            $('#'+td_nome).html('Carregando...');
            img.attr ('src','../imagens/menos.gif');
            exibirListaAtividades(pecid, td_nome);
        }
        if( $('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() != ""){
            $('#'+tr_nome).css('display','');
            img.attr('src','../imagens/menos.gif');
        } else {
            $('#'+tr_nome).css('display','none');
            img.attr('src','/imagens/mais.gif');
        }
    }
</script>

<form action="" method="POST" id="formulario" name="formulario">
    <input type="hidden" id="requisicao" name="requisicao" value=""/>

        <?PHP
            $acao = "
                <img id=\"img_pec_' || pec.pecid || '\" src=\"/imagens/mais.gif\" style=\"cursor: pointer\" onclick=\"carregarAtividadesPEC(this.id,\'' || pec.pecid || '\');\"/>
            ";

            $sql = "
                SELECT  DISTINCT '{$acao}',
                        no_ies,
                        caidsc,
                        no_curso,
                        peccpf,
                        pecnome,
                        pecrne,
                        pecmatricula,
                        pecrendimento,
                        siadesc,
                        '<tr style=\"display:none;\" id=\"listaAtidades_' || pec.pecid || '\" ><td></td><td id=\"trA_' || pec.pecid || '\" colspan=\"10\"></td></tr>' as listaAtividades

                FROM assint.pecg pec

                JOIN assint.bolsa bol ON bol.bolid = pec.bolid
                JOIN assint.habilitacao hab ON hab.habid = pec.habid
                JOIN territorios.pais pai ON pai.paiid = pec.paiid
                JOIN assint.situacaoaluno sia ON sia.siaid = pec.siaid
                JOIN emec.cursos cur ON cur.co_curso = pec.co_curso AND cur.co_ies = pec.co_ies
                JOIN emec.ies ies ON ies.co_ies = pec.co_ies

                JOIN gestaodocumentos.instituicaoensino AS est ON est.iesid = ies.co_ies
                JOIN gestaodocumentos.categoriaadm AS ctg ON ctg.caiid = est.caiid

                JOIN assint.atividadehistorico AS a on a.pecid = pec.pecid

                WHERE a.atvaba = 'P'
                
                ORDER BY 1
            ";
            $cabecalho = array("Ação", "IES", "Nat. IES", "Curso", "CPF", "Nome", "RNE", "Matrícula", "Rendimento Academico", "Situação");
            $alinhamento = Array('center', 'left', 'left', 'left', 'center','left', 'left', 'left', 'center', 'right');
            //$tamanho = Array('5%', '10%', '50%', '10%', '10%', '10%');
            $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);
        ?>
</form>



