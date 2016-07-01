<?php
verificaCodigoTermoSessao();
?>
<link rel="stylesheet" type="text/css" href="css/_termoCooperacao.css" />
<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css" />
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="js/_termoCooperacao.js"></script>
<script type="text/javascript" src="../includes/calendario.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/lib/jquery.metadata.js"></script>
<!-- <script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script> -->
<!-- <script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script> -->
<script type="text/javascript">

    function carregaMunicipios(id, muncod, habilita_cumprimento) {
        $.ajax({
            url		: 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A',
            type	: 'post',
            data	: 'requisicao=carregarMunicipios&estuf='+id+'&muncod='+muncod+'&habilitado='+habilita_cumprimento+'',
            success	: function(data) {
                $('#td_municipios').html(data);
            }
        });
    }

    $(document).ready(function(){

        var camposObrigatorios  = "[name=reccnpj], [name=recnome], [name=recendereco], [name=estuf], [name=reccep],";
        camposObrigatorios += "[name=rectelefoneddd], [name=rectelefone], [name=uocod], [name=ugcod], [name=gestaocod],";
        camposObrigatorios += "[name=recnomeresponsavel], [name=reccpfresponsavel], [name=recsiaperesponsavel], [name=recrgresponsavel], [name=recdtemissaorgresposavel],";
        camposObrigatorios += "[name=recexpedidorrgresposavel], [name=reccargo], [name=recemailresposavel], [name=recnumportaria], [name=recdtpublicacao],";
        camposObrigatorios += "[name=recexecucaoobjeto], [name=recatividadesprevistas], [name=recmetaprevista], [name=recatividadesexecutadas], [name=recmetaexecutada],";
        camposObrigatorios += "[name=recdificuldades], [name=recmetasadotadas], [name=reccomentarios], [name=recvlrrecebido], [name=recvlrutilizado],";
        camposObrigatorios += "[name=recvlrdevolvido], [name=recnumnotacredito[]]";

        $(camposObrigatorios).addClass("required");

        var camposEmails = "[name=recemailresposavel]";

        $(camposEmails).addClass("email");

        //var camposData = "[name=recdtemissaorgresposavel]";
        //$(camposData).addClass("date");

        var camposNumericos = "[name=uocod], [name=ugcod], [name=gestaocod]";
        $(camposNumericos).addClass("number");

        $("#formCumprimento").validate();

        $('.salvarCumprimento').click(function(){

            if($('[name=muncod]').val() == ''){
                alert('O campo município é obrigatório!');
                $('[name=muncod]').focus();
                return false;
            }

            $('#formCumprimento').submit();
            //document.getElementById('formCumprimento').submit();
        });

        $('.inserirNC').click(function(){
            var tr = $("#linhanc1").clone().removeAttr("id");
            tr.find('input').val('');
            tr.children('.acao').html('<img src="../imagens/excluir.gif" class="excluirNC" id="0" style="cursor: pointer"/>');
            tr.insertAfter( $("#gridNC > tbody > tr:last") );
            $("#formCumprimento").validate();
        });

        $('.inserirNCdev').click(function(){
            var tr = $("#linhanc1dev").clone().removeAttr("id");
            tr.find('input').val('');
            tr.children('.acao').html('<img src="../imagens/excluir.gif" class="excluirNC" id="0" style="cursor: pointer"/>');
            tr.insertAfter( $("#gridNCdev > tbody > tr:last") );
            $("#formCumprimento").validate();
        });

        $('.excluirNC').live('click', function(){
            var rcnid = this.id;
            var tr = $(this).parent().parent();
            if(confirm('Deseja excluir esta linha?')){
                if(parseInt(this.id) > 0){
                    $.ajax({
                        url		: 'elabrev.php?modulo=principal/termoCooperacao/cadTermoCooperacao&acao=A&aba=cumprimento',
                        type	: 'post',
                        data	: 'requisicao=excluirNota&rcnid='+rcnid,
                        success	: function(e){
                            console.log(e);
                            if(parseInt(e)>0){
                                tr.remove();
                                alert('Registro excuido com sucesso!');
                            }else{
                                alert('Falha ao excluir resgistro!');
                            }
                        }
                    });
                }else{
                    $(this).parent().parent().remove();
                }
            }
        });

        $(".dowloadDocAnexo").live("click", function(){
            var arqid = $(this).attr("data-target-id");
            if (arqid) {
                $('#arqid').val(arqid);
                $('#requisicao').val('dowloadDocAnexo');
                document.getElementById('formCumprimento').submit();
            }
        });

        $(".excluirDocAnexo").live("click", function(){
            if (confirm("Deseja realmente excluir o Arquivo?")) {
                var arqid = $(this).attr("data-target-id");
                if (arqid) {
                    $('#arqid').val(arqid);
                    $('#requisicao').val('excluirDocAnexo');
                    document.getElementById('formCumprimento').submit();
                }
            }
        });
    });

</script>
<?php

if(!$boPopup) require_once APPRAIZ . 'includes/cabecalho.inc';
echo "<br>";
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo(MODULO_NAME, '');
montaCabecalhoUG(Array('termo'=>true));
echo "<br>";
montaAbaTermoCooperacao();

if($_SESSION['elabrev']['tcpid']){

    $sql = "
		Select	recid, tcpid, reccnpj, recnome, recendereco, muncod, estuf, reccep, 
				substr(rectelefone, 1, 2) as rectelefoneddd,
				substr(rectelefone, 3, 9) as rectelefone,
				uocod, ugcod, gestaocod, recnomeresponsavel, reccpfresponsavel, 
				recsiaperesponsavel, recrgresponsavel, recdtemissaorgresposavel, 
				recexpedidorrgresposavel, reccargo, recemailresposavel, recnumportaria, 
				recdtpublicacao, recnumnotacredito, recexecucaoobjeto, recatividadesprevistas, 
				recmetaprevista, recatividadesexecutadas, recmetaexecutada, recdificuldades, 
				recmetasadotadas, reccomentarios, recvlrrecebido, recvlrutilizado, 
			    recvlrdevolvido, recnumncdevolucao, recstatus
		From elabrev.relatoriocumprimento
		Where
		recstatus = 'A' And
		tcpid = ".$_SESSION['elabrev']['tcpid']."
	";

    $rs = $db->pegaLinha($sql);
    if($rs) {
        extract($rs);
    }
    // Caso não tenha cadastrado relatório de cunprimento do objeto, traz o formulário com o preenchimento padrão
    else {
        if ($rs = preenchimentoPadraoDoObjeto($_SESSION['elabrev']['tcpid'])) {
            extract($rs);
            $reccpfresponsavel = ($reccpfresponsavel) ? formatar_cpf($reccpfresponsavel) : '';
        }
    }
}

if( $habilita_cumprimento == "N" ){
    $habilita = "disabled=disabled";
}else{
    $habilita = "";
}

$reccnpj = $reccnpj ? formatar_cnpj($reccnpj) : '';
$recdtemissaorgresposavel = $recdtemissaorgresposavel ? formata_data($recdtemissaorgresposavel) : '';
$recdtpublicacao = $recdtpublicacao ? formata_data($recdtpublicacao) : '';
$recvlrrecebido = $recvlrrecebido ? formata_valor($recvlrrecebido) : '';
$recvlrutilizado =$recvlrutilizado ? formata_valor($recvlrutilizado) :'';
$recvlrdevolvido =$recvlrdevolvido ? formata_valor($recvlrdevolvido) : '';
$reccep = $reccep ? formata_cep($reccep) : '';
?>

    <form method="post" name="formCumprimento" id="formCumprimento" enctype="multipart/form-data">
    <input type="hidden" name="tcpid" id="tcpid" value="<?php echo $_SESSION['elabrev']['tcpid']; ?>" />
    <input type="hidden" name="arqid" id="arqid" value="" />
    <input type="hidden" name="requisicao" id="requisicao" value="salvarRelCumprimento" />
    <input type="hidden" name="recid" id="recid" value="<?php echo $recid; ?>" />
    <?php if ($habilita_cumprimento == 'N') : ?>
        <input type="hidden" name="onlyread" id="onlyread" value="1" />
    <?php endif; ?>
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
    <tr>
        <td colspan="2" class="subtituloCentro">Dados da Entidade Proponente</td>
    </tr>
    <tr>
        <td class="subtituloDireita" width="40%">CNPJ</td>
        <td><?php echo campo_texto('reccnpj', 'S', $habilita_cumprimento, '', 25, 18, '##.###.###/####-##', '') ?></td>
    </tr>
    <tr>
        <td class="subtituloDireita">Nome da Entidade</td>
        <td><?php echo campo_texto('recnome', 'S', $habilita_cumprimento, '', 45, 255, '', '') ?></td>
    </tr>
    <tr>
        <td class="subtituloDireita">Endereço</td>
        <td><?php echo campo_texto('recendereco', 'S', $habilita_cumprimento, '', 45, 500, '', '') ?></td>
    </tr>
    <tr>
        <td class="subtituloDireita">UF</td>
        <td>
            <?php
            $sql = "select estuf as codigo, estdescricao as descricao from territorios.estado order by estuf";
            $rsUfs = $db->carregar($sql);

            echo '<select name="estuf" id="estuf" class="CampoEstilo"  '.$habilita.' onchange="carregaMunicipios(this.value)" '.($habilita_cumprimento == 'S' ? '' : 'selected="selected"').'><option value="">Selecione...</option>';
            foreach($rsUfs as $data){
                echo '<option value="'.$data['codigo'].'" '.($estuf == $data['codigo'] ? 'selected="selected"' : '').'>'.$data['descricao'].'</option>';
            }
            echo '</select>';
            echo '&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">';
            ?>
        </td>
    </tr>
    <tr>
        <td class="subtituloDireita">Município</td>
        <td id="td_municipios">Selecione um estado primeiro.</td>
    </tr>
    <tr>
        <td class="subtituloDireita">CEP</td>
        <td><?php echo campo_texto('reccep', 'S', $habilita_cumprimento, '', 15, 10, '##.###-###', '') ?></td>
    </tr>
    <tr>
        <td class="subtituloDireita">Telefone com DDD</td>
        <td><?php echo campo_texto('rectelefoneddd', 'S', $habilita_cumprimento, '', 4, 2, '##', '').'&nbsp;
			'.campo_texto('rectelefone', 'S', $habilita_cumprimento, '', 20, 11, '####-####', '') ?></td>
    </tr>
    <tr>
        <td class="subtituloDireita">Código da UO</td>
        <td><?php echo campo_texto('uocod', 'S', $habilita_cumprimento, '', 20, 15, '########', '') ?></td>
    </tr>
    <tr>
        <td class="subtituloDireita">Código da UG</td>
        <td><?php echo campo_texto('ugcod', 'S', $habilita_cumprimento, '', 20, 15, '########', '') ?></td>
    </tr>
    <tr>
        <td class="subtituloDireita">Código da Gestão</td>
        <td><?php echo campo_texto('gestaocod', 'S', $habilita_cumprimento, '', 20, 15, '########', '') ?></td>
    </tr>
    <tr>
        <td class="subtituloDireita">Nome do Responsável</td>
        <td><?php echo campo_texto('recnomeresponsavel', 'S', $habilita_cumprimento, '', 45, 255, '', '') ?></td>
    </tr>
    <tr>
        <td class="subtituloDireita">CPF do Responsável</td>
        <td><?php echo campo_texto('reccpfresponsavel', 'S', $habilita_cumprimento, '', 20, 18, '###.###.###-##', '') ?></td>
    </tr>
    <tr>
        <td class="subtituloDireita">SIAPE do Responsável</td>
        <td><?php echo campo_texto('recsiaperesponsavel', 'S', $habilita_cumprimento, '', 15, 20, '##########', '') ?></td>
    </tr>
    <tr>
        <td class="subtituloDireita">Identidade do Responsável</td>
        <td><?php echo campo_texto('recrgresponsavel', 'S', $habilita_cumprimento, '', 10, 20, '', '') ?></td>
    </tr>
    <tr>
        <td class="subtituloDireita">Data Emissão</td>
        <td>
            <?php
            echo  campo_data('recdtemissaorgresposavel', 'S', $habilita_cumprimento, '', 10, '', '', '', 'formCumprimento');
            // 					$texto_ajuda = '';
            // 					echo campo_data2('recdtemissaorgresposavel','S',$habilita_cumprimento,'','DD/MM/YYYY', $texto_ajuda, '', $recdtemissaorgresposavel, '', null, 'recdtemissaorgresposavel' );
            ?>
        </td>
    </tr>
    <tr>
        <td class="subtituloDireita">Orgão Expedidor</td>
        <td><?php echo campo_texto('recexpedidorrgresposavel', 'S', $habilita_cumprimento, '', 10, 15, '', '') ?></td>
    </tr>
    <tr>
        <td class="subtituloDireita">Cargo</td>
        <td><?php echo campo_texto('reccargo', 'S', $habilita_cumprimento, '', 45, 255, '', '') ?></td>
    </tr>
    <tr>
        <td class="subtituloDireita">E-mail do Responsável</td>
        <td><?php echo campo_texto('recemailresposavel', 'S', $habilita_cumprimento, '', 45, 255, '', '') ?></td>
    </tr>
    <tr>
        <td class="subtituloDireita">Nº da Portaria ou Decreto de Nomeação</td>
        <td><?php echo campo_texto('recnumportaria', 'S', $habilita_cumprimento, '', 25, 150, '', '') ?></td>
    </tr>
    <tr>
        <td class="subtituloDireita">Data de Publicação</td>
        <td><?php echo campo_data('recdtpublicacao', 'S', $habilita_cumprimento, '', 10, '', '', '', 'formCumprimento') ?></td>
    </tr>
    <tr>
        <td colspan="2" class="subtituloCentro">Dados do Objeto da Descentralização do Crédito</td>
    </tr>
    <!--
		<tr>
			<td class="subtituloDireita">Nº do Termo de Execução Descentralizada</td>
			<td><?php echo campo_texto('recnumportaria', 'S', $habilita_cumprimento, '', 25, '', '', '') ?></td>
		</tr>
		-->
    <tr>
        <td class="subtituloDireita">Nota(s) de Crédito</td>
        <td align="left">

            <?php
            if(!empty($_SESSION['elabrev']['tcpid'])){
                $sql = "select rcnid, recid, rcnnumnc from elabrev.relcumprimentonc where tcpid = {$_SESSION['elabrev']['tcpid']} and rcndevolucao = false";
                //ver($sql);
                $rsNCenvio = $db->carregar($sql);
            }
            ?>

            <table id="gridNC" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="left" style="width:340px">
                <thead>
                <tr>
                    <th>&nbsp;</th>
                    <th>Nº da Nota de Crédito</th>
                </tr>
                </thead>
                <tbody id="bodyNC">
                <?php if($rsNCenvio): ?>
                    <?php foreach($rsNCenvio as $k => $v): ?>
                        <tr <?php echo $k==0 ? 'id="linhanc1"' : ''?>>
                            <td class="acao" width="20">
                                <?php if($estadoAtual == EM_EXECUCAO && verificaEquipeTecnicaProponente()):?>
                                    <img src="../imagens/excluir.gif" class="excluirNC" style="cursor: pointer" id="<?php echo $v['rcnid']; ?>"/>
                                <?php else: ?>
                                    &nbsp;
                                <?php endif; ?>
                            </td>
                            <td><?php echo campo_texto('recnumnotacredito[]', 'S', $habilita_cumprimento, '', 25, 25, '', '', '', '', '', '', '', $v['rcnnumnc']) ?></td>
                        </tr>
                    <?php endforeach; ?>
                <?php else: ?>
                    <tr id="linhanc1">
                        <td class="acao" width="20">&nbsp;</td>
                        <td><?php echo campo_texto('recnumnotacredito[]', 'S', $habilita_cumprimento, '', 25, '', '', '') ?></td>
                    </tr>
                <?php endif; ?>
                </tbody>
                <tfoot>
                <tr>
                    <td colspan="2">
                        <img src="../imagens/gif_inclui.gif" style="cursor:pointer" align="absmiddle"/>
                        &nbsp;<a href="javascript:void(0)" class="inserirNC">Inserir nova NC</a>
                    </td>
                </tr>
                </tfoot>
            </table>

        </td>
    </tr>
    <tr>
        <td class="subtituloDireita">Execução do Objeto</td>
        <td>
            <?php
            $arExecucao = array(
                array('codigo'=>1,'descricao'=>'Houve cumprimento TOTAL'),
                array('codigo'=>2,'descricao'=>'Houve cumprimento PARCIAL'),
                array('codigo'=>3,'descricao'=>'Houve devolução integral')
            );

            echo '<select name="recexecucaoobjeto" id="recexecucaoobjeto"  '.$habilita.' class="CampoEstilo" '.($habilita_cumprimento == 'S' ? '' : 'selected="selected"').'><option value="">Selecione...</option>';
            foreach($arExecucao as $data){
                echo '<option value="'.$data['codigo'].'" '.($recexecucaoobjeto == $data['codigo'] ? 'selected="selected"' : '').'>'.$data['descricao'].'</option>';
            }
            echo '</select>';
            echo '&nbsp;<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">';
            ?>

        </td>
    </tr>
    <tr>
        <td class="subtituloDireita">Atividades Previstas</td>
        <td><?php echo campo_textarea('recatividadesprevistas', 'S', $habilita_cumprimento, '', 75, 3, 5000) ?></td>
    </tr>
    <tr>
        <td class="subtituloDireita">Meta Prevista</td>
        <td><?php echo campo_textarea('recmetaprevista', 'S', $habilita_cumprimento, '', 75, 3, 5000) ?></td>
    </tr>
    <tr>
        <td class="subtituloDireita">Atividades Executadas</td>
        <td><?php echo campo_textarea('recatividadesexecutadas', 'S', $habilita_cumprimento, '', 75, 3, 5000) ?></td>
    </tr>
    <tr>
        <td class="subtituloDireita">Meta Executada</td>
        <td><?php echo campo_textarea('recmetaexecutada', 'S', $habilita_cumprimento, '', 75, 3, 5000) ?></td>
    </tr>
    <tr>
        <td class="subtituloDireita">Dificuldades Encontradas na Execução da Descentralização</td>
        <td><?php echo campo_textarea('recdificuldades', 'S', $habilita_cumprimento, '', 75, 3, 5000) ?></td>
    </tr>
    <tr>
        <td class="subtituloDireita">Medidas Adotadas para Sanar as Dificuldades de Modo a Assegurar o Cumprimrnto do Objeto</td>
        <td><?php echo campo_textarea('recmetasadotadas', 'S', $habilita_cumprimento, '', 75, 3, 5000) ?></td>
    </tr>
    <tr>
        <td class="subtituloDireita">Comentários Adicionais</td>
        <td><?php echo campo_textarea('reccomentarios', 'S', $habilita_cumprimento, '', 75, 3, 5000) ?></td>
    </tr>
    <tr>
        <td colspan="2" class="subtituloCentro">Detalhamento do Crédito Orçamentário Recebido</td>
    </tr>
    <tr>
        <td class="subtituloDireita">Valor Recebido (R$ 1,00)</td>
        <td><?php echo campo_texto('recvlrrecebido', 'S', $habilita_cumprimento, '', 25, '', '[.###],##', '') ?></td>
    </tr>
    <tr>
        <td class="subtituloDireita">Valor Utilizado (R$ 1,00)</td>
        <td><?php echo campo_texto('recvlrutilizado', 'S', $habilita_cumprimento, '', 25, '', '[.###],##', '') ?></td>
    </tr>
    <tr>
        <td class="subtituloDireita">Valor Devolvido (R$ 1,00)</td>
        <td><?php echo campo_texto('recvlrdevolvido', 'S', $habilita_cumprimento, '', 25, '', '[.###],##', '') ?></td>
    </tr>
    <tr>
        <td class="subtituloDireita">NC de Devolução</td>
        <td>
            <?php //echo campo_texto('recnumncdevolucao', 'Ns', $habilita_cumprimento, '', 25, '', '', '') ?>
            <?php
            if(!empty($_SESSION['elabrev']['tcpid'])){
                $sql = "select rcnid, recid, rcnnumnc from elabrev.relcumprimentonc where tcpid = {$_SESSION['elabrev']['tcpid']} and rcndevolucao = true";
                //ver($sql);
                $rsNCenvio = $db->carregar($sql);
            }
            ?>
            <table id="gridNCdev" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="left" style="width:340px">
                <thead>
                <tr>
                    <th>&nbsp;</th>
                    <th>Nº da Nota de Crédito de Devolução</th>
                </tr>
                </thead>
                <tbody id="bodyNCdev">
                <?php if($rsNCenvio): ?>
                    <?php foreach($rsNCenvio as $k => $v): ?>
                        <tr <?php echo $k==0 ? 'id="linhanc1dev"' : ''?>>
                            <td class="acao" width="20">
                                <?php if ($estadoAtual == EM_EXECUCAO && verificaEquipeTecnicaProponente()) :?>
                                    <img src="../imagens/excluir.gif" class="excluirNC" style="cursor: pointer" id="<?php echo $v['rcnid']; ?>"/>
                                <?php else: ?>
                                    &nbsp;
                                <?php endif; ?>
                            </td>
                            <td><?php echo campo_texto('recnumnotacredito_dev[]', 'N', $habilita_cumprimento, '', 25, '50', '', '', '', '', '', '', '', $v['rcnnumnc']) ?></td>
                        </tr>
                    <?php endforeach; ?>
                <?php else: ?>
                    <tr id="linhanc1dev">
                        <td class="acao" width="20">&nbsp;</td>
                        <td><?php echo campo_texto('recnumnotacredito_dev[]', 'N', $habilita_cumprimento, '', 25, '50', '', '') ?></td>
                    </tr>
                <?php endif; ?>
                </tbody>
                <tfoot>
                <tr>
                    <td colspan="2">
                        <img src="../imagens/gif_inclui.gif" style="cursor:pointer" align="absmiddle"/>
                        &nbsp;<a href="javascript:void(0)" class="inserirNCdev">Inserir nova NC de devolução</a>
                    </td>
                </tr>
                </tfoot>
            </table>
        </td>
    </tr>
    <tr>
        <td class="subtituloDireita">Anexar arquivo</td>
        <td>
            <input type="file" name="arquivo" id="arquivo" />
        </td>
    </tr>
    <tr>
        <td colspan="2">
            <?php
            if ($recid > 0) {

                $acao = "
                    <img border=\"0\" title=\"Apagar arquivo\" class=\"excluirDocAnexo\" data-target-id=\"'|| arq.arqid ||'\" style=\"cursor: pointer\" align=\"absmiddle\" src=\"../imagens/excluir.gif\" />
                    <img border=\"0\" title=\"Baixar arquivo\" class=\"dowloadDocAnexo\" data-target-id=\"'|| arq.arqid ||'\" style=\"cursor: pointer\" align=\"absmiddle\" src=\"../imagens/indicador-vermelha.png\" />
                    ";

                $desativado = "
                    <img border=\"0\" align=\"absmiddle\" src=\"../imagens/excluir_01.gif\" />
                    <img border=\"0\" title=\"Baixar arquivo\" class=\"dowloadDocAnexo\" data-target-id=\"'|| arq.arqid ||'\" style=\"cursor: pointer\" align=\"absmiddle\" src=\"../imagens/indicador-vermelha.png\" />
                    ";

                $down = "<a title=\"Baixar arquivo\" href=\"#\" class=\"dowloadDocAnexo\" data-target-id=\"'|| arq.arqid ||'\">' || arq.arqnome || '</a>";

                /**
                 * Se o termo já estiver ido alguma vez para execução
                 * não pode excluir anexos, apenas adicionar
                 */
                if (alreadyBeenExecucao($_SESSION['elabrev']['tcpid'])) {
                    $sqlComplement = "SELECT '{$desativado}' AS acao, arq.arqnome AS descricao,";
                } else {
                    $sqlComplement = "SELECT CASE WHEN anx.usucpf = '{$_SESSION['usucpf']}' THEN '{$acao}' ELSE '{$desativado}' END AS acao, '{$down}' AS descricao,";
                }

                $sql = "
                    {$sqlComplement}
                    arq.arqnome||'.'||arq.arqextensao,
                    su.usunome,
                    to_char(arq.arqdata, 'DD/MM/YYYY')
                    FROM elabrev.anexos  anx
                    JOIN public.arquivo arq on (arq.arqid = anx.arqid)
                    JOIN seguranca.usuario su ON (su.usucpf = anx.usucpf)
                    WHERE arq.arqstatus = 'A' AND recid = {$recid}
                    ";
                //dbg(simec_htmlentities($sql));

                $cabecalho = Array('Ação', 'Descrição', 'Nome do arquivo', 'Usuário Responsável', 'Data da Inclusão');
                $align  = Array('center', 'left', 'left', 'left', 'right');
                $db->monta_lista($sql, $cabecalho, 50, 10, 'N', 'left', 'N', '', $whidth, $align, '');
            }
            ?>
        </td>
    </tr>
    <?php
    $rco = new RelatorioCumprimentoObjeto();
    if ($estadoAtual == EM_EXECUCAO && verificaEquipeTecnicaProponente()) : ?>
        <tr>
            <td colspan="2" align="center">
                <input type="button" id="salvarCumprimento" value="Gravar" class="salvarCumprimento"/>
            </td>
        </tr>
    <?php elseif ($rco->termoVencido($_SESSION['elabrev']['tcpid']) && verificaEquipeTecnicaProponente()) : ?>
        <tr>
            <td colspan="2" align="center">
                <input type="button" id="salvarCumprimento" value="Gravar" class="salvarCumprimento"/>
            </td>
        </tr>
    <?php elseif ($estadoAtual == TERMO_EM_DILIGENCIA_RELATORIO && verificaEquipeTecnicaProponente()) : ?>
        <tr>
            <td colspan="2" align="center">
                <input type="button" id="salvarCumprimento" value="Gravar" class="salvarCumprimento"/>
            </td>
        </tr>
    <?php elseif (in_array(PERFIL_SUPER_USUARIO, $perfis)) : ?>
        <tr>
            <td colspan="2" align="center">
                <input type="button" id="salvarCumprimento" value="Gravar" class="salvarCumprimento"/>
            </td>
        </tr>
    <?php endif; ?>
    </table>
    </form>
<?php if($estuf): ?>

    <script type="text/javascript">
        carregaMunicipios('<?php echo $estuf; ?>', '<?php echo $muncod ? $muncod : ''; ?>', '<?php echo $habilita_cumprimento; ?>');
    </script>

<?php endif; ?>


<script type="text/javascript">
    setTimeout(function(){
        if ($("select[name='muncod_disable']").length) {
            $("input[name='muncod']").val($("select[name='muncod_disable']").val());
        }
    }, 500);
</script>