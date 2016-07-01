<?
    
    $perfil = pegaPerfilGeral();
    
    if( in_array(PERFIL_CADASTROGERAL, $perfil) || in_array(PERFIL_SUPERUSUARIO, $perfil) ){
	$habilita = 'S';
        $icone = "<img border=\"0\" src=\"../imagens/alterar.gif\" style=\"cursor:pointer\" title=\"Alterar\" class=\"alterar\" id=\"".$camp['cnuid']."\">";
    }else{
        $icone = "<img border=\"0\" src=\"../imagens/alterar_01.gif\" >";
    }

    if( $_SESSION['academico']['entid'] != '' ){
        $sql = "
            SELECT  nunid, 
                    entidunidade,
                    to_char(numdtcriacao, 'DD/MM/YYYY') as numdtcriacao
            FROM academico.novasuniversidades
            WHERE entidunidade = {$_SESSION['academico']['entid']}
        ";
        $dados = $db->pegaLinha($sql);
    }
    
    #FORMATA A DATA PADRÃO BR PARA O PADRÃO AMERICANO (PREPARA PARA O BANCO DE DADOS).
    function formataDataBanco($valor){
        $data = explode("/",$valor);
        $dia = $data[0];
        $mes = $data[1];
        $ano = $data[2];
        return $ano."-".$mes."-".$dia;	
    }
    
    function salvar() {
        global $db;

        $numdtcriacao   = $_REQUEST['numdtcriacao'] == '' ? 'NULL' : "'".formataDataBanco( $_REQUEST['numdtcriacao'] )."'";
        
        if( $numdtcriacao != '' ){
            $sql = "
                UPDATE academico.novasuniversidades SET numdtcriacao = {$numdtcriacao} WHERE entidunidade = {$_SESSION['academico']['entid']} RETURNING nunid;
            ";
            $dados = $db->executar($sql);
        }

        if( is_array($_POST['cnurealizado']) && $dados > 0 ) {
            $sql = '';
            foreach ($_POST['cnurealizado'] as $cnuid => $valor) {

                $_POST['cnurealizado'][$cnuid]  = $_POST['cnurealizado'][$cnuid] ? $_POST['cnurealizado'][$cnuid] : '0.00';
                $_POST['cnuobs'][$cnuid]        = $_POST['cnuobs'][$cnuid] ? $_POST['cnuobs'][$cnuid] : '';

                $sql .= "
                    UPDATE academico.campusnovuniversidade
                        SET cnurealizado    = '{$_POST['cnurealizado'][$cnuid]}',
                            cnuobs          = '{$_POST['cnuobs'][$cnuid]}'
                        WHERE cnuid = $cnuid RETURNING cnuid;
                ";
            }
            $dados = $db->executar($sql);

            if( $dados > 0 ){
                $db->commit();
                $db->sucesso( 'principal/monitoramentoProgramasAcoes&acao=A&aba=NovaUniversidade', '' );
            }else{
                $db->insucesso('Não foi possivél gravar o Dados, tente novamente mais tarde!', '', 'principal/monitoramentoProgramasAcoes&acao=A&aba=NovaUniversidade');
            }
        }
    }

    if ($_REQUEST['req']) {
        $_REQUEST['req']();
        die();
    }

    monta_titulo('Nova Universidade', 'Atualizar dados da Universidade');

?>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
    $(document).ready(function() {
        $('.alterar').click(function() {
            var entid = $(this).attr('id');
            if ($(this).attr('src') == '../imagens/alterar_pb.gif') {
                $(this).attr('src', '../imagens/alterar.gif');
                $(this).attr('title', 'Editar');                
                $('[name*="[' + entid + ']"]').attr('disabled', true);
                $('[name*="[' + entid + ']"]').attr('readonly', true);
                $('[name*="[' + entid + ']"]').removeClass('normal');
                $('[name*="[' + entid + ']"]').addClass('disabled');
            } else {
                $(this).attr('src', '../imagens/alterar_pb.gif');
                $(this).attr('title', 'Bloquear');
                $('[name*="[' + entid + ']"]').attr('disabled', false);
                $('[name*="[' + entid + ']"]').attr('readonly', false);
                $('[name*="[' + entid + ']"]').removeClass('disabled');
                $('[name*="[' + entid + ']"]').addClass('normal');
            }
        });

        $('.salvar').click(function() {
            $('#req').val('salvar');
            $('#formulario').submit();
        });
    });
</script>

<form id="formulario" name="formulario" method="post" action="">
    <input type="hidden" value="" name="req" id="req" />
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
         <tr>
            <td valign="top" class="SubtituloDireita" width="25%" >Data da Criação FIES:</td>
            <td>
                <?PHP
                    $numdtcriacao = $dados['numdtcriacao'];
                    echo campo_texto('numdtcriacao', 'N', $habilitado, '', 10, 10, '##/##/####', '', '', '', '', 'id="numdtcriacao"', '', $numdtcriacao);
                ?>
            </td>
         </tr>
    </table>
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td>
                <table cellspacing="0" cellpadding="2" border="0" align="center" width="100%" class="listagem" style="color:333333;">
                    <?PHP
                        $sql = "
                            SELECT  cnuid, 
                                    cnu.nunid, 
                                    cnudsc, 
                                    cnuprevisto, 
                                    cnurealizado, 
                                    cnuobs
                            FROM academico.novasuniversidades nu
                            JOIN academico.campusnovuniversidade AS cnu ON cnu.nunid = nu.nunid
                            WHERE nu.entidunidade = {$_SESSION['academico']['entid']}
                        ";
                        $campus = $db->carregar($sql); 
                        $cabecalho = Array('Ação', 'Campus', 'Previsto', 'Realizado', 'Observação');

                        if (is_array($campus)) {
                    ?>
                    
                    <tr>
                        <? foreach ($cabecalho as $cab) { ?>
                            <td align="center" valign="top" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" class="title">
                                    <?= $cab ?>
                            </td>
                        <? } ?>
                    </tr>
                    
                    <?PHP foreach ($campus as $camp) { ?>
                            <tr bgcolor="" onmouseout="this.bgColor = '';" onmouseover="this.bgColor = '#ffffcc';">
                                <td valign="top" title="Ação" align="center">
                                    <?=$icone;?>
                                </td>
                                <td valign="top" title="Campus">
                                    <?= $camp['cnudsc'] ?>
                                    <input type="hidden" name="cnuid[]" id="cnuid_<?= $camp['cnuid'] ?>" value="<?= $camp['cnuid'] ?>">
                                </td>
                                <td valign="top" title="Ação" align="center">
                                    <?= campo_texto('cnuprevisto[]', 'N', 'N', '', 6, 6, '####.#', '', '', '', '', 'id="cnuprevisto_'. $camp['cnuid'].'"', '', $camp['cnuprevisto']); ?>
                                </td>
                                <td valign="top" title="Ação" align="center">
                                    <?= campo_texto("cnurealizado[{$camp['cnuid']}]", 'N', 'N', '', 6, 6, '####.#', '', '', '', '', 'id="cnurealizado_'.$camp['cnuid'].'"', '', $camp['cnurealizado']); ?>
                                </td>
                                <td valign="top" title="Ação" align="center">
                                    <?= campo_texto("cnuobs[{$camp['cnuid']}]", 'N', 'N', '', 50, 255, '', '', '', '', '', 'id="cnuobs_'.$camp['cnuid'] . '"', '', $camp['cnuobs']); ?>
                                </td>
                            </tr>
                    <?PHP
                        }
                    }
                    ?>
                </table>
            </td>
        </tr>
        <tr>
            <td>
                <?PHP
                    if($habilita == 'S'){
                ?>
                    <input type="button" class="salvar" style="cursor:pointer" value="Salvar">
                <?PHP
                    }else{
                ?>
                    <input type="button" style="cursor:pointer" value="Salvar" disabled="disabled">
                <?PHP } ?>
            </td>
        </tr>    
    </table>
</form>
