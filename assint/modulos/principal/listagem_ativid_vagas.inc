
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script type="text/javascript">

//#CARREGAR LISTA FILHO
    function exibirListaAtividades(ofpid, td_nome){
	divCarregando();
	$.ajax({
            type    : "POST",
            //url     : window.location.href,
            url     : 'assint.php?modulo=principal/PEC_G&acao=A&aba=listagem_ativid_vagas',
            data    : "requisicao=exibirListaAtividades&ofpid="+ofpid,
            async   : false,
            success: function(msg){
                $('#'+td_nome).html(msg);
                divCarregado();
            }
	});
    }

    //#CARREGAR LISTA FILHO
    function carregarAtividadesVagas(idImg, ofpid){
        var img     = $( '#'+idImg );
        var tr_nome = 'listaAtidades_'+ ofpid;
        var td_nome = 'trA_'+ ofpid;

        if( $('#'+tr_nome).css('display') == 'none' && $('#'+td_nome).html() == "" ){
            $('#'+td_nome).html('Carregando...');
            img.attr ('src','../imagens/menos.gif');
            exibirListaAtividades(ofpid, td_nome);
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
        
            $ano = $_REQUEST['ano'];
            
            if( $ano != '' ){
                $where = " AND ofe.ofpano = '{$ano}' ";
            }
        
            $acao = "
                <img id=\"img_vagas_' || ofe.ofpid || '\" src=\"/imagens/mais.gif\" style=\"cursor: pointer\" onclick=\"carregarAtividadesVagas(this.id,\'' || ofe.ofpid || '\');\"/>
            ";

            $sql = "
                
                SELECT  DISTINCT '{$acao}'  AS acao,
                        ies.no_ies,
                        cur.no_curso,
                        ofe.ofpiescampus,
                        ofe.ofpano,
                        REPLACE(ofe.ofpanosemestre, '.', ' - ') AS ofpanosemestre,
                        hab.habdesc,
                        '<tr style=\"display:none;\" id=\"listaAtidades_' || ofe.ofpid || '\" ><td></td><td id=\"trA_' || ofe.ofpid || '\" colspan=\"10\"></td></tr>' as listaAtividades
                        
                FROM assint.ofertacursopecg ofe

                JOIN emec.cursos cur ON cur.co_curso = ofe.co_curso
                JOIN assint.habilitacao hab ON hab.habid = ofe.habid
                JOIN emec.ies ies ON ies.co_ies = ofe.co_ies AND cur.co_ies = ies.co_ies
                
                JOIN assint.atividadehistorico AS a on a.ofpid = ofe.ofpid

                WHERE a.atvaba = 'V' {$where}

                ORDER BY 1
            ";
                
            $coluna_ano = array(
                'label' => 'Ano',
                'colunas' => array(
                    'Edição do Programa', 'Semestre de Ingresso'
                )
            );
            $cabecalho = array('Ação', 'Instituição', 'Curso', 'Campus', $coluna_ano, 'Habilitação' );
            
            $alinhamento = Array('center', 'left', 'left', 'left', 'center', 'center', 'left', 'left', 'center' );
            $tamanho = Array('3%', '30%', '20%', '15%', '6%', '6%', '8%', '5%', '5%', '5%' );
            
            $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'center', 'N', '', $tamanho, $alinhamento);
        ?>
</form>



