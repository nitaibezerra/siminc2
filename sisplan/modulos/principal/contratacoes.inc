<?php
$permissao_formulario = 'S';
if ($_POST['act'] == 'excluir' && $_REQUEST['conid']){
    $contratacao = new Contratacao($_REQUEST['conid']);

    $dado = array('constatus' => 'I');
    $contratacao->popularDadosObjeto($dado)
                ->salvar();
    $contratacao->commit();

    $db->sucesso( $_REQUEST['modulo'],"&atiid={$_REQUEST['atiid']}");
    die();
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( $titulo_modulo, '' );

extract($_REQUEST);

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link type="text/css" href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" rel="stylesheet"></link>
<style type="text/css">
.divCor{
    height: 17px;
    width: 17px;
    margin-right: 2px;
    vertical-align:top;
    display: inline-block;
    border: 1px solid #FFFFFF;
}

label{
}

</style>

<form method="POST" name="formulario" action="">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="SubTituloDireita">Plano Interno</td>
            <td><?= campo_texto('atinumeropi','N','S',$permissao_formulario,53,11,'','','','','',''); ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Número do Processo IPHAN:
            </td>
            <td>
                <?= campo_texto('conumerocprod','N',$permissao_formulario,'',53,20,'#####.######/####-##',''); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Unidade Executora:
            </td>
            <td><?php
                $sql = "SELECT * FROM planointerno.unidadeexecutora_uo WHERE TRUE ";
                $db->monta_combo("uexid",$sql,$permissao_formulario,'&nbsp','','','','','','uexid');
            ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Situação do processo de contratação:
            </td>
            <td><?php
                $sql = "SELECT stpid AS codigo, stpdsc AS descricao FROM sisplan.situacaoprocesso WHERE stpstatus = 'A' ORDER BY 2";
                $db->monta_combo("stpid",$sql,$permissao_formulario,'&nbsp','','','','','','stpid');
            ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Forma de Contratação(Modalidade):
            </td>
            <td>
                <select style="width: 395px;" class="CampoEstilo" name="itrid">
                    <option value=""></option>
                    <optgroup label="Execução Direta">
                        <?php
                            $sql = sprintf( "select itrid, itrdsc from sisplan.instrumento where tpiid = 2 order by itrdsc asc" );
                        ?>
                        <?php foreach( $db->carregar( $sql ) as $tipo ): ?>
                        <option value="<?= $tipo['itrid'] ?>" <? if ($itrid == $tipo['itrid'] ) echo 'selected'; ?> ><?= $tipo['itrdsc'] ?></option>
                        <?php endforeach; ?>
                    </optgroup>
                    <optgroup label="Execução Indireta">
                        <?php
                            $sql = sprintf( "select itrid, itrdsc from sisplan.instrumento where tpiid = 1 order by itrdsc asc" );
                        ?>
                        <?php foreach( $db->carregar( $sql ) as $tipo ): ?>
                        <option value="<?= $tipo['itrid'] ?>" <? if ($itrid == $tipo['itrid'] ) echo 'selected'; ?> ><?= $tipo['itrdsc'] ?></option>
                        <?php endforeach; ?>
                    </optgroup>
                </select>
            </td>
        </tr>


        <tr>
            <td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Ação:</td>
            <td><?php
                $sql = array();
                if ( usuario_possui_perfil( PERFIL_COORDENADOR_ACAO_SIGPLAN ) )
                {
                    $sql = "
                    SELECT
                        ur.acacod
                    FROM sisplan.usuarioresponsabilidade ur
                    WHERE ur.usucpf = '". $_SESSION['usucpf'] . "'
                      AND ur.acacod is not null
                      AND ur.rpustatus = 'A'";

                    $acacodArr = (array) $db->carregarColuna($sql);
                    if ($acacodArr[0]){
                        $acacodWhere = " AND acacod IN ('" . ( implode("','", $acacodArr) ) . "')";
                        $sql = sprintf( "SELECT distinct acacod as codigo, acacod || ' - ' || acadsc AS descricao FROM monitora.acao WHERE acasnrap = false AND prgano = '%d' AND acacod IN ('" . ( implode("','", $acacodArr) ) . "') ORDER BY 2", $_SESSION['exercicio'] );
                    }
                }

                if ( $db->testa_superuser() ||
                     usuario_possui_perfil( PERFIL_APOIO_COORDENADOR_PLANEJAMENTO ) ||
                     usuario_possui_perfil( PERFIL_COORDENADOR_PLANEJAMENTO ) ||
                     usuario_possui_perfil( PERFIL_APOIO_COORDENADOR_ORCAMENTO ) ||
                     usuario_possui_perfil( PERFIL_COORDENADOR_ORCAMENTO ) ) {
                    $sql = sprintf( "SELECT distinct acacod as codigo, acacod || ' - ' || acadsc AS descricao FROM monitora.acao WHERE acasnrap = false AND prgano = '%d' ORDER BY 2", $_SESSION['exercicio'] );
                }

                $db->monta_combo("acacod",$sql,$permissao_formulario,'&nbsp','','','','600','','acacod');
            ?></td>
        </tr>


        <tr>
            <td class="SubTituloDireita">
                Data de Início:
            </td>
            <td>
                <?php
                    if($condatainicio1) {
                        $condatainicio1 = formata_data_sql($condatainicio1);
                    }
                    echo campo_data2( 'condatainicio1', 'N', $permissao_formulario, '', 'S', '' );
                ?>
                &nbsp;a&nbsp;
                <?php
                    if($condatainicio2) {
                        $condatainicio2 = formata_data_sql($condatainicio2);
                    }
                    echo campo_data2( 'condatainicio2', 'N', $permissao_formulario, '', 'S', '' );
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Data Prevista de Término:
            </td>
            <td>
                <?php
                    if($condatatermino1) {
                        $condatatermino1 = formata_data_sql($condatatermino1);
                    }
                    echo campo_data2( 'condatatermino1', 'N', $permissao_formulario, '', 'S', '' );
                ?>
                &nbsp;a&nbsp;
                <?php
                    if($condatatermino2) {
                        $condatatermino2 = formata_data_sql($condatatermino2);
                    }
                echo campo_data2( 'condatatermino2', 'N', $permissao_formulario, '', 'S', '' ); ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Fase com atraso:
            </td>
            <td>
                <input type="radio" name="faseAtraso" id="faseAtrasoS" value="sim" <?= $faseAtraso == 'sim' ? "checked='checked'" : '' ?>/><label for="faseAtrasoS">Sim</label>
                <input type="radio" name="faseAtraso" id="faseAtrasoN" value="nao" <?= $faseAtraso == 'nao' ? "checked='checked'" : '' ?>/><label for="faseAtrasoN">Não</label>
                <input type="radio" name="faseAtraso" id="faseAtrasoT" value="todas" <?= $faseAtraso == 'todas' || $faseAtraso == '' ? "checked='checked'" : '' ?>/><label for="faseAtrasoT">Todas</label>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">
                Situação do acompanhamento:
            </td>
            <td>
                <input type="checkbox" name="situacaoConcluida" id="situacaoConcluida" value="concluida" <?= $situacaoConcluida == 'concluida' ? 'checked="checked"' : ''; ?>  /><label for="situacaoConcluida"><span class="divCor" style="background-color: blue;" title="Concluída">&nbsp;&nbsp;&nbsp;&nbsp;</span>Concluída</label>
                <input type="checkbox" name="situacaoEstavel"   id="situacaoEstavel"   value="estavel"   <?= $situacaoEstavel   == 'estavel'   ? 'checked="checked"' : ''; ?>  /><label for="situacaoEstavel"><span class="divCor" style="background-color: green;" title="{labeltexto}">&nbsp;&nbsp;&nbsp;&nbsp;</span>Estável</label>
                <input type="checkbox" name="situacaoAtencao"   id="situacaoAtencao"   value="atencao"   <?= $situacaoAtencao   == 'atencao'   ? 'checked="checked"' : ''; ?>  /><label for="situacaoAtencao"><span class="divCor" style="background-color: #DADF00;" title="{labeltexto}">&nbsp;&nbsp;&nbsp;&nbsp;</span>Merece Atenção</label>
                <input type="checkbox" name="situacaoCritico"   id="situacaoCritico"   value="critico"   <?= $situacaoCritico   == 'critico'   ? 'checked="checked"' : ''; ?>  /><label for="situacaoCritico"><span class="divCor" style="background-color: red;" title="{labeltexto}">&nbsp;&nbsp;&nbsp;&nbsp;</span>Crítico</label>
            </td>
        </tr>
        <tr style="background-color: #cccccc">
            <td align='right' style="vertical-align:top; width:25%"> </td>
            <td>
                <input type="submit" name="pesquisar" value="Pesquisar"/>
                <input type="button" name="limpar" value="Limpar" onclick="limparFormulario();"/>
            </td>
        </tr>
    </table>
    <input type="hidden" name="conid" value="""/>
    <input type=hidden   name="act"   value="0"/>
</form>
<?php
    $where = array();
    // Número de Processo
    if($conumerocprod){
        $where[] = "con.conumerocprod = '{$conumerocprod}'";
    }

    // Unidade Executora
    if($uexid){
        $where[] = "ati.uexid = {$uexid}";
    }

    // Situação do processo
    if($stpid){
        $where[] = "aco.stpidacompanhamento = {$stpid}";
    }

    // Forma de Contratação
    if($itrid){
        $where[] = "con.itrid = '{$itrid}'";
    }

    // Ação
    if($acacod){
        $where[] = "aca.acacod = '{$acacod}'";
    }

    // Data Início
    if($condatainicio1){
        $where[] = "con.condatainicio >= '{$condatainicio1}'";
    }

    // Data Início
    if($condatainicio2){
        $where[] = "con.condatainicio <= '{$condatainicio2}'";
    }

    // Data Término
    if($condatatermino1){
        $where[] = "con.condatatermino >= '{$condatatermino1}'";
    }

    // Data Término
    if($condatatermino2){
        $where[] = "con.condatatermino <= '{$condatatermino2}'";
    }

    // Número PI
    if($atinumeropi){
        $where[] = "ati.atinumeropi = '{$atinumeropi}'";
    }

    // Fase com atraso
    if($faseAtraso){
        $contratacao  = new Contratacao();
        $contratacoes = $contratacao->getContratacoesAtrasadas();
        if($faseAtraso == 'sim'){
            if(is_array($contratacoes)){
                $contratacoes = implode(',', $contratacoes);
                $where[] = "con.conid IN ({$contratacoes})";
            }else{
                $where[] = "FALSE";
            }
        }elseif($faseAtraso == 'nao'){
            if(is_array($contratacoes)){
                $contratacoes = implode(',', $contratacoes);
                $where[] = "NOT con.conid IN ({$contratacoes})";
            }else{
                $where[] = "TRUE";
            }
        }
    }

    // Situação do acompanhamento

    $whereOr = array();
    if($situacaoConcluida){
        $whereOr[] = "numtotalf.numtotalfases = numcc.numconcluidacancelada";
    }
    if($situacaoEstavel){
        $whereOr[] = "numamenormenos1.numatrasomenormenos1 > 0";
    }
    if($situacaoAtencao){
        $whereOr[] = "numamenor10.numatrasomenor10maiormenos1 > 0";
    }
    if($situacaoCritico){
        $whereOr[] = "numamaior10.numatrasomaior10 > 0";
    }

    if(count($whereOr) > 0){
        $where[] = "(" . implode(" OR ", $whereOr) . ")";
    }

    $contratacao = new Contratacao();
    $rs          = $contratacao->getByWhere2($where);

    $arConfig = array("style"           => "width:95%;",
                      "totalLinha"      => false,
                      "totalRegistro"   => true);

    $arCabecalho = array("Ações",
                         "Situação",
                         "Ação",
                         "Número do processo",
                         "Objeto",
                         "Modalidade",
                         "Situação",
                         "Valor Previsto",
                         "Responsável",
                         "Data de Início",
                         "Data Prevista de Término",
                         "Data de Cadastro",
                         "Data do Último Acompanhamento");

    $acao = '<center>
                <img src=\'/imagens/alterar.gif\' style="cursor:pointer" title="Alterar" onclick="editar( {conid}, {atiid} )">
                <img src=\'/imagens/excluir.gif\' style="cursor:pointer" title="Excluir" onclick="excluir( {conid} )">
                <img src="/imagens/consultar.gif" style="cursor: pointer" onclick="abrirDadosPA({atiid})" title="Tramitação / Abrir Plano de Ação "/>
                <img src="/imagens/gif_inclui.gif" style="cursor:pointer;" border="0" title="Incluir Acompanhamento" align="top"  onclick="incluirAcomp( {conid}, {atiid} )">
            </center>';

    $arParamCol[2] = array("type" => Lista::TYPESTRING);

    $oLista = new Lista($arConfig);
    $oLista->setCabecalho( $arCabecalho );
    $oLista->setCorpo( $rs, $arParamCol );
    $oLista->setAcao( $acao );
    $oLista->show();
?>

<script type="text/javascript">
    function editar(cod, atiid){
        window.location = '?modulo=principal/dadosContratacao&acao=A&atiid='+atiid+'&conid='+cod;
    }

    function incluirAcomp(cod, atiid){
        window.location = '?modulo=principal/dadosAcompanhamento&acao=A&atiid='+atiid+'&conid='+cod;
    }

    function excluir(cod){
        if ( confirm('Tem certeza que deseja excluir esta Contratação?') ){
            document.formulario.act.value = 'excluir';
            document.formulario.conid.value = cod;

            document.formulario.submit();
        }
    }

    function abrirDadosPA( id )
    {
        window.open(
        '?modulo=principal/dadospa&acao=C&atiid=' + id,
        'dadospa',
        'width=670,height=580,scrollbars=yes,scrolling=yes,resizebled=yes'
        );
    }

    function limparFormulario(){
        document.formulario.conumerocprod.value   = '';
        document.formulario.uexid.value           = '';
        document.formulario.stpid.value           = '';
        document.formulario.itrid.value           = '';
        document.formulario.condatainicio1.value  = '';
        document.formulario.condatainicio2.value  = '';
        document.formulario.condatatermino1.value = '';
        document.formulario.condatatermino2.value = '';
        document.getElementById('faseAtrasoT').checked = 'checked';
        document.getElementById('situacaoConcluida').checked = '';
        document.getElementById('situacaoEstavel').checked = '';
        document.getElementById('situacaoAtencao').checked = '';
        document.getElementById('situacaoCritico').checked = '';
    }

</script>