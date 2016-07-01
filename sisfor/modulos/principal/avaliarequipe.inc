<?php
verificarFluxoComposicaoEquipe($_SESSION['sisfor']['docidcomposicaoequipe']);

$estadoAtual['esdid'] = null;
if ($_REQUEST['fpbid']) {
	
	$_REQUEST['fpbid'] = str_replace(array("#"),array(""),$_REQUEST['fpbid']);
    $docid = pegarDocidFolhaPagamentoProjeto($_SESSION['sisfor']['sifid'], $_REQUEST['fpbid']);
    $estadoAtual = wf_pegarEstadoAtual($docid);
}

$podeEditar = ESD_AVALIACAO == $estadoAtual['esdid'] ? 'S' : 'N';

if ($_REQUEST['fpbid']) {

    $mensarios = recuperarMensarios($_SESSION['sisfor']['sifid'], $_REQUEST['fpbid']);

    if ($podeEditar=='S') {
        // Recuperando todos os envolvidos no curso
        $sql = "insert into sisfor.mensario (tpeid, fpbid, mensatatus)
                select t.tpeid, '{$_REQUEST['fpbid']}', 'A'
                from sisfor.tipoperfil t 
                left join sisfor.mensario m on m.tpeid = t.tpeid and m.fpbid={$_REQUEST['fpbid']}
                where t.sifid = '{$_SESSION['sisfor']['sifid']}'
                and tpestatus = 'A' and m.menid is null";

        $db->executar($sql);
        $db->commit();

        $mensarios = recuperarMensarios($_SESSION['sisfor']['sifid'], $_REQUEST['fpbid']);
    }
}

if ($_REQUEST['excluir_mensario']) {
    if ($_REQUEST['menid']) {
        $sql = "delete from sisfor.mensarioavaliacoes where menid = {$_REQUEST['menid']}";
        $db->executar($sql);

        $sql = "delete from sisfor.mensario where menid = {$_REQUEST['menid']}";
        $db->executar($sql);

        $db->commit();

        $location = "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso_execucao&acao=A&aba=avaliarusuario&fpbid={$_REQUEST['fpbid']}";
        $al = array("alert" => "Operação realizada com sucesso!", "location" => $location);
        alertlocation($al);
    }
}

if ('gravar' == $_REQUEST['acao']) {
    if (isset($_REQUEST['avaliacao']) && is_array($_REQUEST['avaliacao'])) {
        foreach ($_REQUEST['avaliacao'] as $avaliacao) {
            $menid = $avaliacao['menid'] ? $avaliacao['menid'] : null;
            $mavid = $avaliacao['mavid'] ? $avaliacao['mavid'] : null;
//            $mavparticipacao = $avaliacao['mavparticipacao'] ? str_replace(array('.', ','), array('', '.'), $avaliacao['mavparticipacao']) : 0;
            $mavatividadesrealizadas = $avaliacao['mavatividadesrealizadas'] ? str_replace(array('.', ','), array('', '.'), $avaliacao['mavatividadesrealizadas']) : 0;
            $iusdorientador = $avaliacao['iusdorientador'] ? $avaliacao['iusdorientador'] : null;
            $motivoavaliacao = $avaliacao['motivoavaliacao'] ? $avaliacao['motivoavaliacao'] : null;
//            $mavtotal = $avaliacao['mavtotal'] ? str_replace(array('.', ','), array('', '.'), $avaliacao['mavtotal']) : null;

            if($menid && !$mavid) {
            	$mavid = $db->pegaUm("select mavid from sisfor.mensarioavaliacoes where menid={$menid}");
            }

            if ($mavid) {
                $sql = " update sisfor.mensarioavaliacoes set
                                iusdorientador = ".(($iusdorientador)?"'".$iusdorientador."'":"NULL").",
                                mavatividadesrealizadas = '$mavatividadesrealizadas',
                                motivoavaliacao = ".(($motivoavaliacao)?"'".substr($motivoavaliacao,0,100)."'":"NULL")."
                         where mavid = $mavid ";
            } else {
                $sql = " INSERT INTO sisfor.mensarioavaliacoes(iusdorientador, menid, mavatividadesrealizadas, motivoavaliacao)
                                VALUES (".(($iusdorientador)?"'".$iusdorientador."'":"NULL").", $menid, '$mavatividadesrealizadas', ".(($motivoavaliacao)?"'".substr($motivoavaliacao,0,100)."'":"NULL").") ";
            }

            $db->executar($sql);
            $db->commit();
        }
    }

    $location = "sisfor.php?modulo=principal/coordenador_curso/coordenador_curso_execucao&acao=A&aba=avaliarusuario&fpbid={$_REQUEST['fpbid']}";
    $al = array("alert" => "Operação realizada com sucesso!", "location" => $location);
    alertlocation($al);
}
?>

<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td class="SubTituloCentro" colspan="2">Avaliar Equipe</td>
    </tr>
    <tr>
        <td class="SubTituloDireita" width="20%">Orientações</td>
        <td>
        <p>A avaliação da equipe é uma etapa fundamental para habilitar o recebimento da bolsa. Escolha o período de referência. O sistema exibirá a lista de todos os bolsistas cadastrados.</p> 
		<p>Informe, para cada bolsista, se as atividades previstas foram realizadas ou não e por quem ele(a) foi avaliado. Observe que, neste momento, o sistema supõe que o Coordenador do Curso não tem condições de avaliar sozinho toda a equipe e somente fará o registro de quem efetivamente fez o julgamento quanto às atividades.</p>
		<p>Ao assinalar a opção “Realizou as atividades”, o sistema automaticamente assume a condição de bolsista “Apto”. Caso a situação seja “Não realizou as atividades”, a condição passa para “Não apto” e, neste caso, é obrigatório informar o que motivou esta opção (ou seja, que atividades não foram realizadas). Neste caso, não será gerada nenhuma solicitação de pagamento.</p>
		<p>Observe que há também a opção “Não se aplica”, que deve ser utilizada SOMENTE no caso dos bolsistas que, naquele período de referência, não tinham atividades previstas e, portanto, não podem ser avaliados. Também neste caso, não será gerada nenhuma solicitação de pagamento.</p>
		<p>Dúvidas  ou problemas com o sistema? Envie um e-mail para <?php echo $_SESSION['email_sistema']; ?>.</p>
        </td>
    </tr>
    <tr>
        <td colspan="2" class="SubTituloCentro">
<?
carregarPeriodoReferencia(array('sifid' => $_SESSION['sisfor']['sifid'], 'fpbid' => $_REQUEST['fpbid']));
?>
        </td>
    </tr>


    <tr>
        <td colspan="2">
<? if ($funcaoavaliacao == 'carregarAvaliacaoEquipe') : ?>
                <form method=post name="formbuscar" id="formbuscar">
                    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                        <tr><td class="SubTituloDireita" width="25%">CPF</td><td><?= campo_texto('filtro[iuscpf]', "N", "S", "CPF", 15, 14, "###.###.###-##", "", '', '', 0, 'id="iuscpf"', '', $_REQUEST['iuscpf']); ?></td></tr>
                        <tr><td class="SubTituloDireita" width="25%">Nome</td><td><?= campo_texto('filtro[iusnome]', "N", "S", "Nome", 67, 60, "", "", '', '', 0, 'id="iusnome"', '', $_REQUEST['iusnome']); ?></td></tr>
                        <tr><td class="SubTituloDireita" width="25%">Perfil</td><td><?
    $sql = "SELECT pfldsc as codigo, pfldsc as descricao FROM seguranca.perfil WHERE pflcod IN('" . PFL_FORMADORIES . "','" . PFL_SUPERVISORIES . "','" . PFL_COORDENADORADJUNTOIES . "','" . PFL_COORDENADORLOCAL . "','" . PFL_ORIENTADORESTUDO . "','" . PFL_PROFESSORALFABETIZADOR . "','" . PFL_COORDENADORIES . "')";
    $db->monta_combo('filtro[pfldsc]', $sql, 'S', 'Selecione', 'selecionarPerfilGerenciar', '', '', '200', 'N', 'pfldsc', '', $_REQUEST['pfldsc']);
    ?>
                            </td></tr>
                        <tr><td class="SubTituloCentro" colspan=2><input type="button" name="buscar" value="Buscar" onclick="document.getElementById('formbuscar').submit();"><input type="button" name="vertodos" value="Ver todos" onclick="window.location = window.location;"></td></tr>
                    </table>
                </form>
            <? endif; ?>

            <?php if ($_REQUEST['fpbid']) { ?>

                <?php if ('S' == $podeEditar) { ?>
                    <form method="post" id="formulario_avaliacao" name="formulario_avaliacao">
                    <?php } ?>
                    <input type="hidden" name="acao" value="gravar" />
                    <?php $sqlOrientador = "    select iu.iusd as codigo, iusnome as descricao
                                                                     from sisfor.identificacaousuario iu
                                                                            inner join sisfor.tipoperfil tp on tp.iusd = iu.iusd
                                                                        where sifid = '{$_SESSION['sisfor']['sifid']}'
                                                                        order by descricao"; ?>
                    
                    <table width="100%">
                        <tr>
                            <td width="95%" valign="top">

                                <table cellspacing="0" cellpadding="2" border="0" align="center" width="95%" class="listagem tabela" style="margin-top: 10px;">
                                    <thead>
                                        <tr align="center">
                                            <td class="SubTituloCentro">CPF</td>
                                            <td class="SubTituloCentro">Nome</td>
                                            <td class="SubTituloCentro">E-mail</td>
                                            <td class="SubTituloCentro">Perfil</td>
                                            <td class="SubTituloCentro">Atividades Realizadas</td>
                                            <td class="SubTituloCentro">Avaliado por</td>
                                            <td class="SubTituloCentro">Situação</td>
                                        </tr>
                                        <tr align="center">
                                            <td colspan="4" align="right"><b>Selecionar todos</b></td>
                                            <td><? $db->monta_combo('todosatividadesrealizadas', opcoesComboAtividadesEquipe(), $podeEditar, 'Selecione', '', '', '', '', 'N', 'todosatividadesrealizadas', '', '', '','style="font-size:x-small;"'); ?></td>
                                            <td><? $db->monta_combo('todosorientador', $sqlOrientador, $podeEditar, 'Selecione', '', '', '', '', 'N', 'todosorientador', '', '', '','style="font-size:x-small;"'); ?></td>
                                            <td>&nbsp;</td>
                                        </tr>
                                    </thead>
                                    <tbody>
                                        <?php
                                        if ($mensarios && is_array($mensarios)) {
                                            foreach ($mensarios as $count => $dado) {
                                                ?>
                                                <tr>
                                                    <td style="font-size:x-small;">
                                                        <input type="hidden" name="avaliacao[<?php echo $count; ?>][menid]" value="<?php echo $dado['menid'] ?>" />
                                                        <input type="hidden" name="avaliacao[<?php echo $count; ?>][mavid]" value="<?php echo $dado['mavid'] ?>" />
            <?php echo $dado['iuscpf'] ?>
                                                    </td>
                                                    <td style="font-size:x-small;"><?php echo $dado['iusnome'] ?></td>
                                                    <td style="font-size:x-small;"><?php echo $dado['iusemailprincipal'] ?></td>
                                                    <td style="font-size:x-small;"><?php echo $dado['pfldsc'] ?></td>
                                                    <td align="center">
            <?php
            $db->monta_combo("avaliacao[{$count}][mavatividadesrealizadas]", opcoesComboAtividadesEquipe(), $podeEditar, 'Selecione', '', '', '', '200', 'N', '', '', $dado['mavatividadesrealizadas'], '', 'item_count="' . $count . '" style="font-size:x-small;"', 'cb_atividades_realizadas situacao situacao_' . $count);
            ?>
                                                    </td>
                                                    <td align="center">
                                                        <? $db->monta_combo("avaliacao[$count][iusdorientador]", $sqlOrientador, $podeEditar, 'Selecione', '', '', '', '200', 'N', '', '', $dado['iusdorientador'], '', ' style="font-size:x-small;"', 'cb_orientador'); ?>
                                                    </td>
                                                    <td align="center">
                                                        <input type="hidden" id="motivoavaliacao_<?php echo $count; ?>" value="<?php echo $dado['motivoavaliacao'] ?>"><span id="situacao_bolsista_<?php echo $count; ?>"></span>
            <?php
            if ('N' == $podeEditar){
                $motivo = '';
                $situacao =  ('A' == trim($dado['mavatividadesrealizadas'])) ? '<span style="font-size:x-small; color: green;"><b>APTO</b></span>' : '-';
                if ('I' == trim($dado['mavatividadesrealizadas'])) {
                    $situacao =  '<span style="font-size:x-small; color: red;"><b>NÃO APTO</b></span>';
                    $motivo = '. Motivo: ' . $dado['motivoavaliacao'];
                }
                echo $situacao;
                echo $motivo;
            }
            ?>
                                                    </td>
                                                </tr>
                                            <?php } ?>
                                            <tr>
                                                <td align="center" bgcolor="#CCCCCC" colspan="9">
                                                    <?php if ('S' == $podeEditar) { ?>
                                                        <input type="button" pflcod="<?php echo $pflcod; ?>" class="botao_enviar" id="botao_enviar"  name="salvar" value="Salvar">
                                                    <?php } ?>
                                                </td>
                                            </tr>
                                        <?php } else { ?>
                                            <tr bgcolor="" onmouseout="this.bgColor = '';" onmouseover="this.bgColor = '#ffffcc';">
                                                <td colspan="4" align="center"><span style="color: red;">Não foi encontrado planejamento de equipe</span></td>
                                            </tr>
                                        <?php } ?>
                                    </tbody>
                                </table>

                            </td>
                            <td width="5%" valign="top">
                                <?
                                /* Barra de estado atual e ações e Historico */
                                $docid = pegarDocidFolhaPagamentoProjeto($_SESSION['sisfor']['sifid'], $_REQUEST['fpbid']);
                                wf_desenhaBarraNavegacao($docid, array('sifid' => $_SESSION['sisfor']['sifid'], "fpbid" => $_REQUEST['fpbid']));
                                ?>
                            </td>
                        </tr>
                    </table>
                    <?php if ('S' == $podeEditar) { ?>
                    </form>
                <?php } ?>
            <?php } ?>
        </td>
    </tr>
</table>

<? if($_GET['fpbid']) : ?>
<div style="font-weight: bold;text-align: center;text-align: center;font-size: 14px;padding: 20px">Situação dos pagamentos por bolsista</div>
            
<table class="listagem" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<tr>
	<td class="SubTituloCentro">Bolsista</td>
	<td class="SubTituloCentro">Perfil</td>
	<td class="SubTituloCentro">Pagamento gerado</td>
	<td class="SubTituloCentro">Situação pagamento</td>
	<td class="SubTituloCentro">Restrição</td>
</tr>
<?
	if($mensarios[0]) {
		foreach($mensarios as $me) {

			if($me['mavatividadesrealizadas']=='A') {
				$vagas = $db->pegaLinha("SELECT tpebolsa, tpeqtdbolsa as qtdprevista, (select count(*) from sisfor.pagamentobolsista p inner join workflow.documento d on d.docid = p.docid where p.tpeid=t.tpeid and d.esdid!=".ESD_PAGAMENTO_NAO_AUTORIZADO.") as qtdutilizada FROM sisfor.tipoperfil t WHERE tpeid='".$me['tpeid']."'");
				
				$pagamento = $db->pegaLinha("SELECT p.pboid, e.esddsc FROM sisfor.pagamentobolsista p 
											 INNER JOIN workflow.documento d ON d.docid = p.docid 
											 INNER JOIN workflow.estadodocumento e ON e.esdid = d.esdid 
										     INNER JOIN sisfor.tipoperfil t ON t.tpeid = p.tpeid  
										     WHERE p.iusd='".$me['iusd']."' AND p.fpbid='".$me['fpbid']."' AND t.tpeid='".$me['tpeid']."'");
				unset($restricao);
				$termocompromisso = $db->pegaUm("SELECT iustermocompromisso FROM sisfor.identificacaousuario WHERE iusd='".$me['iusd']."'");
				

				if(!$pagamento) {
					if ($termocompromisso!='t') {
						$restricao .= '<span style="font-size:x-small;color:red;">Termo de compromisso não aceito</span>';
					} elseif($vagas['tpebolsa']!='t') {
						$restricao .= '<span style="font-size:x-small;color:red;">Não deseja receber bolsa do curso</span>';
					} else {
						$erros = verificarCriacaoPagamento(array('tpeid' => $me['tpeid'],'fpbid' => $_GET['fpbid']));
						
						if($erros) {
							$restricao .= '<span style="font-size:x-small;color:red;">';
							$restricao .= implode("<br>",$erros);
		        			$restricao .= '</span>';
	        			}
					}
				}

				echo '<tr>';
				echo '<td>'.$me['iuscpf'].' - '.$me['iusnome'].'</td>';
				echo '<td>'.$me['pfldsc'].'</td>';
				echo '<td align="center">'.(($pagamento)?'<span style="font-size:x-small;color:blue;">Sim</span>':'<span style="font-size:x-small;color:red;">Não</span>').' <span style="font-size:x-small;"><b>( '.(($vagas['qtdutilizada'])?$vagas['qtdutilizada']:'0').' / '.(($vagas['qtdprevista'])?$vagas['qtdprevista']:'0').' )</b></span></td>';
				echo '<td align="center">'.(($pagamento)?'<span style="color:blue;font-size:x-small;">'.$pagamento['esddsc'].'</span>':'<span style="color:red;font-size:x-small;">Pagamento não foi gerado</span>').'</td>';
				echo '<td align="center" width="30%">'.(($restricao)?$restricao:'<span style="font-size:x-small;">Não possui restrições</span>').'</td>';
				echo '<tr>';
			
			}
		}
	}
?>
</table>
<? endif; ?>

<script type="text/javascript" src="/estrutura/js/funcoes.js"></script>
<script type="text/javascript">
                                        jQuery(function() {
                                            jQuery('#todosparticipacao').change(function() {
                                                jQuery('.cb_participacao').val(jQuery(this).val()).change();
                                            });

                                            jQuery('#todosatividadesrealizadas').change(function() {
                                                jQuery('.cb_atividades_realizadas').val(jQuery(this).val()).change();
                                            });

                                            jQuery('#todosorientador').change(function() {
                                                jQuery('.cb_orientador').val(jQuery(this).val()).change();
                                            });

                                            jQuery('.situacao').live('change', function() {
                                                var item = jQuery(this).attr('item_count');



                                                situacao = '-';
                                                switch (jQuery(this).val()) {
                                                    case 'A':
                                                        situacao = '<span style="color: green;">APTO</span>';
                                                        break;
                                                    case 'I':
                                                        situacao = '<span style="color: red;">NÃO APTO <input style="width:250px;" placeholder="Descreva o motivo" required="required" type="text" name="avaliacao[' + item + '][motivoavaliacao]" id="avaliacao_' + item + '"></span>';

                                                        break;
                                                }

                                                jQuery('#situacao_bolsista_' + item).html(situacao);



                                                jQuery('#avaliacao_' + item).val(jQuery('#motivoavaliacao_' + item).val());


                                            }).change();

                                            jQuery('#botao_enviar').click(function() {
                                                var validado = 1;
                                                jQuery('.obrigatorio').each(function() {
                                                    if (jQuery(this).val() == '')
                                                    {
                                                        validado = 0;
                                                    }
                                                });
                                                if (validado == 1) {
                                                    jQuery('#formulario_avaliacao').submit();
                                                } else {

                                                    alert("Favor preencher os campos obrigatórios")
                                                }
                                            });

                                        });
</script>