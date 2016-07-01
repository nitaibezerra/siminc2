<?php
verificaSessao('obra');

header('Content-type: text/html; charset=ISO-8859-1');

function pegaAcaoWF($esdid) {
    global $db;
    
    $strSQL = "select ae.aedid from workflow.estadodocumento ed
                left join workflow.acaoestadodoc ae ON (ae.esdidorigem = ed.esdid)
                where ed.esdid = {$esdid} limit 1";
    $return = $db->pegaUm($strSQL);
    return ($return) ? $return : false;
}

if (isset($_GET['obrid'])) {
    
    extract($_GET);
    $tpdid = 169;
    
    if (isset($action)) {
        switch ($action) {
            case 'CriaTramite':
                
                $docid = wf_cadastrarDocumento($tpdid, '');
                if (wf_alterarEstado($docid, 2474, '', array())) {
                    
                    $arDado = array(
                        'obrid' => $obrid,
                        'docid' => $docid,
                        'oaejustificativa' => strip_tags($_POST['oaejustificativa']),
                    );

                    $tramitaExclusao = new ObraAditivoExclusao();
                    $tramitaExclusao->popularDadosObjeto($arDado)->salvar(true, true, array());
                    $tramitaExclusao->clearDados();
                    $message = 'Operação concluída com sucesso!';
                    $db->commit();
                    
                } else {
                    $message = 'Falha ao tentar concluir a operação, entre em contato com o adminitrador do sistema!';
                }
                die("<script type=\"text/javascript\">
                        alert(\"".$message."\");
                        window.opener.location.reload();
                        self.close();
                    </script>");
                break;
            case 'AlteraEstado':

                $tramitaExclusao = new ObraAditivoExclusao();
                $registro = $tramitaExclusao->pegaLinhaPorObrid($obrid);
                $acaoID = pegaAcaoWF($esdid);
                //ver($acaoID); exit;
                
                if (wf_alterarEstado($registro['docid'], $acaoID, '', array('obrid' => $obrid, 'oaeid' => $registro['oaeid']))) {
                    $message = 'Operação concluída com sucesso!';
                } else {
                    $message = 'Falha ao tentar concluir a operação, entre em contato com o adminitrador do sistema!';
                }
                die("<script type=\"text/javascript\">
                        alert(\"".$message."\");
                        window.opener.location.reload();
                        self.close();
                    </script>");
                break;
        }
    }
    
    $tramitaExclusao = new ObraAditivoExclusao();

    // Verifica se a obra possuia um aditivo excluido, se ela possuir um novo aditivo, habilita a possibilidade de exclusão novamente.
    $result = $tramitaExclusao->pegaLinhaPorObrid($obrid);
    $dados_wf = wf_pegarEstadoAtual($result['docid']);
    $obra     = new Obras( $obrid );
    $crtid    = $obra->pegaContratoPorObra( $obrid );
    $contrato = new Contrato( $crtid );
    $dados    = $contrato->getDados();
    $novaExclusao = ($dados_wf['esdid'] == 1054 && $dados['ttaid']) ? true : false;


    if ($docid = $tramitaExclusao->possuiDocid($obrid) && !$novaExclusao) {
        $estadoAtual = wf_pegarEstadoAtual($docid);
        $estadoInicial = false;        
        extract($tramitaExclusao->pegaLinhaPorObrid($obrid));
        
        //$aditivoExclusao = new ObraAditivoExclusao($oaeid);
        //$history = wf_pegarHistorico($docid);
        //ver($history);
        //ver($aditivoExclusao);
    } else {
        $dados_wf = wf_pegarEstadoInicial($tpdid);
        $estadoInicial = true;
    }

    ?>
    <script type="text/javascript" src="../includes/JQuery/jquery-1.9.1/jquery-1.9.1.js"></script>
    <script language="JavaScript" src="../includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    <script type="text/javascript">
        $(function(){
            $(".i-alterarEstadoWf").on("click", function(){
                if (!$("#oaejustificativa").val()) {
                    alert("O preechimento do campo Justificativa é obrigatório!");
                    return false;
                }
                
                $.ajax({
                    url:'/obras2/obras2.php?modulo=principal/excluirAditivoObra&acao=A&obrid=<?php echo $obrid; ?>&action=CriaTramite'
                  , type: 'POST'
                  , data:{obrid:<?=$obrid?>, oaejustificativa:$("#oaejustificativa").val()}
                  , beforeSend:function(){}
                  , success:function(data){ $("body").append(data); }
                  , complete:function(){}
                  , error:function(){}
                });
            });
            
            $(".alterarEstadoWf").on("click", function(){
                if (confirm("Voce tem certeza que deseja realizar essa operação?")) {
                   $.ajax({
                       url:'/obras2/obras2.php?modulo=principal/excluirAditivoObra&acao=A&obrid=<?php echo $obrid; ?>&esdid='+$("#esdid").val()+'&action=AlteraEstado'
                     , beforeSend:function(){}
                     , success:function(data){ $("body").append(data); }
                     , complete:function(){}
                     , error:function(){}
                   });
                } else {
                    return false;
                }
            });
        });
    </script>
    <?php
    
    monta_titulo($titulo_modulo, '');
    echo cabecalhoObra($obrid);
    
    $habilitado = ($estadoInicial) ? 'S' : 'E';
    $disabled = ($estadoInicial) ? '' : 'disabled="disabled"';
    if(possui_perfil(PFLCOD_CALL_CENTER)){
        $habilitado = 'N';
        $disabled   = 'disabled="disabled"';
        
    }
    echo '<form method="POST">';
    echo '<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">';
    echo '<tr><td valign="top" class="subtitulodireita">Justificativa</td>';
    echo '<td>';
    echo campo_textarea('oaejustificativa', 'S', $habilitado, '', '70', '8', '5000', '' , 0, '', '', false, '', '');
    echo '<input type="hidden" value="'.$esdid.'" id="esdid" name="esdid">';
    echo '</td></tr>';
    echo '<tr><td colspan="2" align="center">';
    echo '<input type="button" class="i-alterarEstadoWf" '.$disabled.' value="Salvar">';
    echo '</td></tr>';
    echo '</table>';
    echo '</form>';
    
    if ($habilitado == 'E') {
        echo '<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">';
        echo '<tr>';
        echo '<td><strong style="font-size:15px;">Deseja realmente proceder com a exclusão do aditivo para esta obra?</strong>&nbsp;&nbsp;<input type="button" class="alterarEstadoWf" value="Sim"></td>';
        echo '</tr>';
        echo '</table>';
    }
    
}