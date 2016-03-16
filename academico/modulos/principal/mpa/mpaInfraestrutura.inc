<?PHP
    unset($_SESSION['arrayObras']);
    
    function itensTipologias( $dados ){
        global $db;
        
        header("Content-Type: text/html; charset=ISO-8859-1");
        
        $dinid = $dados['dinid'];
        
        $sql = "
            SELECT  d.tpdid, 
                    t.tpddsc, 
                    t.tdpdetalhamento
            FROM academico.demandatipologia d
            JOIN academico.tipologiademanda AS t ON t.tpdid = d.tpdid
            WHERE d.dinid = {$dinid}
            ORDER BY 2
        ";
        $dados =$db->carregar($sql);
        
        if($dados){
            $titulo .= '<table style="width: 100%;" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">';
            $titulo .= '<tr><th>Cód.</th><th>Descrição</th><th>Detalhamento</th></tr>';
            $x = 0;						
            foreach($dados as $dado){
                    if( ($x % 2) == 0 ){
                            $cor = "#FFFAFA";
                    }else{
                            $cor = "#D3D3D3";
                    }
                    $titulo .= '<tr style="background-color: '.$cor.';">
                                    <td>'.$dado['tpdid'].'</td>
                                    <td>'.$dado['tpddsc'].'</td>
                                    <td>'.$dado['tdpdetalhamento'].'</td>
                                </tr>';
                    $x = $x + 1;
            }
            $titulo .= '</table>';
        }else{
            $titulo = 'Não há informações!';
        }
        echo $titulo;            
    }

    function listarInfra(){
        global $db;
        
        header("Content-Type: text/html; charset=ISO-8859-1");
        
        $entid_campus = $_REQUEST['entid'];
        
        #USUÁRIO - DA SESSÃO.
        $usucpf = $_SESSION['usucpf'];
        
        $perfil = pegaPerfilGeral();
    
        if( !(in_array( PERFIL_SUPERUSUARIO, $perfil ) || in_array( PERFIL_ADMINISTRADOR, $perfil ))  ){
            $acao = "
                <img border=\"0\" src=\"../imagens/alterar.gif\" title=\"Alterar\" style=\"cursor: pointer\" class=\"alterar\" id=\"'|| din.dinid ||'\">
                <img border=\"0\" src=\"../imagens/excluir.gif\" title=\"Excluir\" style=\"cursor: pointer\" class=\"excluir\" id=\"'|| din.dinid ||'\">
            ";
            $acao_d = "
                <img border=\"0\" src=\"../imagens/alterar_01.gif\" title=\"Alterar\" style=\"cursor: pointer\" class=\"alterar\" id=\"'|| din.dinid ||'\">
                <img border=\"0\" src=\"../imagens/excluir_01.gif\" title=\"Excluir\" style=\"cursor: pointer\" class=\"excluir\" id=\"'|| din.dinid ||'\">
            ";
        }else{
            $acao = "
                <img border=\"0\" src=\"../imagens/alterar.gif\" title=\"Alterar\" style=\"cursor: pointer\" class=\"alterar\" id=\"'|| din.dinid ||'\">
                <img border=\"0\" src=\"../imagens/excluir.gif\" title=\"Excluir\" style=\"cursor: pointer\" class=\"excluir\" id=\"'|| din.dinid ||'\">
            ";
            $acao_d = "
                <img border=\"0\" src=\"../imagens/alterar.gif\" title=\"Alterar\" style=\"cursor: pointer\" class=\"alterar\" id=\"'|| din.dinid ||'\">
                <img border=\"0\" src=\"../imagens/excluir.gif\" title=\"Excluir\" style=\"cursor: pointer\" class=\"excluir\" id=\"'|| din.dinid ||'\">
            ";
        }

        $sql = "
            SELECT CASE WHEN din.usucpf = '{$usucpf}'
                        THEN '{$acao}' 
                        ELSE '{$acao_d}'
                    END AS acao,
                    CASE
                        WHEN dintipo = 'I' THEN '<img border=\"0\" src=\"../imagens/icones/ly.png\" title=\"Outros\" >' 
                        WHEN dintipo = 'O' THEN '<img border=\"0\" src=\"../imagens/icones/lb.png\" title=\"Obras\" >' 
                    END as Leng_tipo,
                    dindsc as descricao, 
                    ' '||dinano||' ',
                    CASE WHEN ( dintipo = 'O' AND speid IS NOT NULL ) 
                        THEN '<img border=\"0\" src=\"../imagens/check_p.gif\" title=\"Outros\" >' 
                        ELSE ''
                    END as projeto_exec,
                    din.dinvlr2014,
                    din.dinvlr2015,
                    din.dinvlr2016,
                    din.dinvlr2017,
                    ( dinvlr2014 + dinvlr2015 + dinvlr2016 + dinvlr2017  ) AS SOMA,
                    '<span onmouseout=\"tipologiasInfoHidden();\" onmouseover=\"tipologiasInfoVisibilyt('|| tp.dinid ||');\" style=\"cursor: pointer\">'|| tp.qtd_tipol ||'</span>' as qtd_tipol,
                    '<span onmouseout=\"obrasInfoHidden();\" onmouseover=\"obrasInfoVisibilyt('|| dob.dinid ||');\" style=\"cursor: pointer\">'|| dob.qtd_obras ||'</span>' as qtd_obras,
                    to_char(dindtprevisaolicitacao, 'DD/MM/YYYY') AS dindtprevisaolicitacao,
                    u.usunome
                    
            FROM academico.demandainfra din 
            LEFT JOIN academico.demandatipologia dtp ON dtp.dinid = din.dinid
            LEFT JOIN academico.tipologiademanda tpd ON tpd.tpdid = dtp.tpdid
            LEFT JOIN seguranca.usuario u ON u.usucpf = din.usucpf

            LEFT JOIN(
                SELECT COUNT(tpdid) qtd_tipol, dinid 
                FROM academico.demandatipologia
                GROUP BY dinid
            )AS tp ON tp.dinid = din.dinid
         
            LEFT JOIN(
                SELECT COUNT(obrid) qtd_obras, dinid 
                FROM academico.demandaobras
                GROUP BY dinid
            ) AS dob ON dob.dinid = din.dinid

            WHERE dinstatus = 'A' AND entidcampus = '{$entid_campus}'

            GROUP BY din.usucpf, din.dinid, dindsc, dinano, dintipo, din.dinvlr2014, din.dinvlr2015, din.dinvlr2016, din.dinvlr2017, projeto_exec, dindtprevisaolicitacao, u.usunome, tp.dinid, tp.qtd_tipol, dob.dinid, dob.qtd_obras
            ORDER BY dindsc
        ";
        $cabecalho = array("Ação", "Tipo", "Descrição", "Ano Ref.", "P.E", "Valor 2014", "Valor 2015", "Valor 2016", "Valor 2017", "Valor Total", "QTD de Tipologias", "QTD de Obras", "Data Previsão do Término", "Usuário");
        $alinhamento = Array('center', 'center', 'left', 'center', 'center', 'center', 'right', 'right', 'right', 'right', 'right','center', 'center');
        $tamanho = Array( '3%', '2%','30%', '4%', '2%', '5%', '5%', '5%', '5%', '5%', '5%', '5%', '5%', '13%'  );
        $db->monta_lista($sql, $cabecalho, 50, 10, 'S', 'center', 'S', '', $tamanho, $alinhamento);
    }	

    function excluir(){
        global  $db;
        if( $_REQUEST['dinid'] != '' ){
            $sql = "UPDATE academico.demandainfra SET dinstatus = 'I' WHERE dinid = ".$_REQUEST['dinid'];
            
            if( $db->executar($sql) ){
                $db->commit();
                $db->sucesso( 'principal/monitoramentoProgramasAcoes&acao=A', '' ); ///academico.php?modulo=principal/monitoramentoProgramasAcoes&acao=A
            }else{
                $db->insucesso('Não foi possivél gravar o Dados, tente novamente mais tarde!', '', 'principal/monitoramentoProgramasAcoes&acao=A');
            }
        }
    }

    if( $_REQUEST['req'] ){
        ob_clean();
        $_REQUEST['req']($_REQUEST);
        die();
    }
    
    monta_titulo('Demanda/Infraestrutura', '');
    
?>

<style>
    .div_info{
        border-style: 1px;
        border-color: black;
        font-family: arial, serif, sans-serif;
        font-size: 18px;
        background-color: #D3D3D3;
        visibility: hidden;
        position: absolute;
        width: 28%;
        height:auto;
        z-index: 100;
    }
</style>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
    $(document).ready(function(){
        
	$('.inserir').click(function(){
            //var janela = window.open( 'academico.php?modulo=principal/mpa/popupMpaInfraestrutura&acao=A', 'adiciona_campus', 'width=1000,height=800,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
            var janela = window.open( 'academico.php?modulo=principal/mpa/popupMpaInfraestrutura&acao=A', 'adiciona_campus', 'fullscreen=1,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
            janela.focus();
	});
        
	$('.excluir').live('click',function(){
            var confirma = confirm("Deseja realmente excluir o Registro?");
            if( confirma ){
                $('#req').val('excluir');
                $('#dinid').val( $(this).attr('id') );
                $('#formulario').submit();
            }
        });
        
        $('.alterar').live('click',function(){
            var dinid = $(this).attr('id');
            //var janela = window.open( 'academico.php?modulo=principal/mpa/popupMpaInfraestrutura&acao=A&dinid='+id, 'adiciona_campus', 'width=1000,height=800,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
            var janela = window.open( 'academico.php?modulo=principal/mpa/popupMpaInfraestrutura&acao=A&dinid='+dinid, 'adiciona_campus', 'fullscreen=1, status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
            janela.focus();
        });
        
	$('.abre').live('click',function(){
            var entid = $(this).attr('id');
            
            $('#entidcampus').val(entid);
            
            if( $('#td'+entid).html() != null ){
                if( $('#tr'+entid).css('display') == 'none' ){
                        $(this).attr('src','../imagens/menos.gif');
                        $('#tr'+entid).show();
                }else{
                        $(this).attr('src','../imagens/mais.gif');
                        $('#tr'+entid).hide();
                }
            }else{
                $(this).attr('src','../imagens/menos.gif');

                var linhaNova = '<tr id="tr'+entid+'"><td id="td'+entid+'" colspan="4" ></td</tr>';

                $(this).parent().parent().parent().after(linhaNova);

                $.ajax({
                    type: "POST",
                    url: window.location,
                    data: "req=listarInfra&entid="+entid,
                    async: false,
                    success: function(msg){ 
                            $('#td'+entid).html(msg);
                    }
                });
            }
		
        });
    });
    
    //Variaveis Globais - Pega a posição do mouse.
    var Tleft = 0;
    var Ttopo = 0;

    $("html").mousemove(function(mouse) {
        Tleft = mouse.pageX - 450;
        Ttopo = mouse.pageY - 100;
    });

    function tipologiasInfoVisibilyt( dinid ) {
        $.ajax({
            type: "POST",
            url: window.location,
            data: "req=itensTipologias&dinid=" + dinid,
            async: false,
            success: function(msg) {
                $(".div_info").css({visibility: "visible"});
                $(".div_info").html(msg);
                $('.div_info').css("top", Ttopo);
                $('.div_info').css("left", Tleft);
            }
        });
    }

    function tipologiasInfoHidden() {
        $(".div_info").css({visibility: "hidden"});
    }
 
</script>

<form id="formulario" name="formulario" method="post" action="">
    <input type="hidden" value="" name="req" id="req" />
    <input type="hidden" value="" name="dinid" id="dinid" />
    <input type="hidden" value="" name="dintipo" id="dintipo" />
    <input type="hidden" value="" name="entidcampus" id="entidcampus" />
    
    <div class="div_info"> </div>
    
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" >
        <tr>
            <td class="SubTituloEsquerda" colspan="2">Legenda</td>
        </tr>
        <tr>
            <td>
                <img border="0" align="absmiddle" src="../imagens/icones/lb.png" title="Demandas do tipo Obras" > Demandas do tipo Obras. 
                &nbsp;&nbsp;&nbsp;
                <img border="0" align="absmiddle" src="../imagens/icones/ly.png" title="Demandas do tipo Outros" > Demandas do tipo Outros.
                &nbsp;&nbsp;&nbsp;
                <img border="0" align="absmiddle" src="../imagens/check_p.gif" title="Demandas do tipo Outros" > P.E: Projeto Executivo Existente.
            </td>
        </tr>
    </table>
    <br>
    
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td>
                <?PHP
                    $acao = "
                        <center>
                            <img border=\"0\" src=\"../imagens/mais.gif\" title=\"Exibir\" class=\"abre\" name=\"campus_'|| e.entid ||'\" id=\"'|| e.entid ||'\" style=\"cursor:pointer\">
                        </center>
                    ";
                    $sql = "
                        SELECT DISTINCT '{$acao}' as acao,
                               entnome,
                               mundescricao||' - '||mun.estuf as municipio,
                               count(dinid) as qtd 
                        FROM academico.demandainfra exp
                        INNER JOIN entidade.entidade e ON e.entid = exp.entidcampus
                        LEFT JOIN entidade.endereco ed ON ed.entid = e.entid
                        LEFT JOIN territorios.municipio mun ON mun.muncod = ed.muncod
                        WHERE entidunidade = '{$_SESSION['academico']['entid']}' AND dinstatus = 'A'
                        GROUP BY e.entid, e.entnome, mun.mundescricao, mun.estuf
                        ORDER BY entnome
                    ";
                    $cabecalho = array("<center>Ação</center>", "Campus", "Município", "N° de Demanda");
                    $alinhamento = Array('center', 'left', 'left', 'left');
                    $tamanho = Array('5%', '70%', '20%', '5%');
                    $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $tamanho, $alinhamento);
                ?>
            </td>
        </tr>
        <tr>
            <td>
                <input type="button" class="inserir" value="Nova infraestrutura">
            </td>
        </tr>    
    </table>
</form>

