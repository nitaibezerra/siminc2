<?php

class Ted_Model_LoteMacro extends Modelo
{
	
	/**
	 * Nome da tabela especificada
	 * @var string
	 * @access protected
	 */
	protected $stNomeTabela = 'ted.lotemacro';
	
	/**
	 * Chave primaria.
	 * @var array
	 * @access protected
	 */
	protected $arChavePrimaria = array('lotid');
	
	/**
	 * Atributos
	 * @var array
	 * @access protected
	*/
	protected $arAtributos = array(
			'lotid' => null,
			'lotdsc' => null,
			'lotdata' => null,
			'lotstatus' => null,
			'lotcpfresponsavel' => null
	);
	
	/**
	 * Campos Obrigatórios da Tabela
	 * @name $arCampos
	 * @var array
	 * @access protected
	*/
	protected $arAtributosObrigatorios = array(
			'lotdsc',
			'lotdata',
			'lotcpfresponsavel'
	);
	
	public function pegaListaMacro() {
		global $db;
		
		$sql = 'select lotid as codigo, lotid as descricao from ted.lotemacro order by lotid';
		
		$list = $this->carregar($sql);
		$options = array();
		if ($list) {
			foreach($list as $item) {
				$options[$item['codigo']] = $item['descricao'];
			}
		}
		
		return ($options) ? $options : array();
	}
	
	public function getWhereListaMacros(){
		$where = array();
		
		if ($_REQUEST['lotid']) {
			$where [] = " lot.lotid = {$_REQUEST['lotid']} ";
		}
		
		if ($_REQUEST['lotdsc']) {
			$where [] = " lot.lotdsc like '%{$_REQUEST['lotdsc']}%' ";
		}
		
		if ($_REQUEST['lotdata']){
			$where [] = " lot.lotdata = '{$_REQUEST['lotdata']}' ";
		}
		
		if ($_REQUEST['usucpf']) {
			$cpf = str_replace(".","",str_replace("-","",$_REQUEST['usucpf']));
			$where [] = " lot.lotcpfresponsavel = '{$cpf}' ";
		}
		
		if ($_REQUEST['usunome']) {
			$nome = strtoupper($_REQUEST['usunome']);
			$where [] = " upper(usu.usunome) like '%{$nome}%' ";
		}
		
		return $where;
	}
	
	public function getQueryListaMacros($where = array(), $joins = array()){
		
		if (count($where) > 0) {
			$_where = " WHERE ".implode(" AND ",$where);
		}
		
		if (count($joins) > 0) {
			$_joins = implode(" ",$joins);
		}
		
		$sql = "SELECT
					'' as acao,
					lot.lotdsc,
					to_char(lot.lotdata, 'DD/MM/YYYY') as lotdata,
					lot.lotcpfresponsavel,
					usu.usunome,
					lot.lotid
				FROM ted.lotemacro lot
				INNER JOIN seguranca.usuario usu ON usu.usucpf = lot.lotcpfresponsavel
				{$_joins}
				{$_where}
				order by lot.lotid desc";
		
		return $sql;
	}
	
	public function getListaMacros($where = array())
    {
		$sql = $this->getQueryListaMacros($where);
        //ver($sql,d);
		
		$list = new Simec_Listagem();
		$list->setCabecalho(array(
		    'N° dos Termos',
		    'Data',
		    'CPF',
		    'Responsável'))
		  ->setQuery($sql);
		$list->esconderColunas (array('lotid'));
		$list->addCallbackDeCampo(array('lotdsc', 'usunome'), 'alinhaParaEsquerda');
		$list->addAcao ( 'view', array(
				'func' => 'visualizar',
				'extra-params' => array ( 'lotid' )
		));
		$list->addAcao ( 'download', array(
				'func' => 'geraXls',
				'extra-params' => array ( 'lotid' )
		));
		$list->turnOnPesquisator();
		
		$list->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
	}
	
	public function geraListaMacros()
    {
		global $db;
		
		// Recupera termos para geração das macros
		//$rs = recuperarLoteTermos();
		$rs = recuperarLoteTermosTeste();
        //ver($rs, d);
		
		// Recupera quantidade maxima de previsão orçamentária do termo
		//$qtdMaxPrevisao = contaQtdMaxPrevisaoOrcamentaria();
		$qtdMaxPrevisao = contaQtdMaxPrevisaoOrcamentariaTeste();
		
		$arMesNum = array('/01/','/02/','/03/','/04/','/05/','/06/','/07/','/08/','/09/','/10/','/11/','/12/');
		$arMesDsc = array('/jan/','/fev/','/mar/','/abr/','/mai/','/jun/','/jul/','/ago/','/set/','/out/','/nov/','/dez/');
		
		if($rs): ?>
		
		    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
		    <script>
		        $(function(){
		
		            largura = (98*$(window).width())/100;
		            $('#divConteudo').css('width', largura);
		
		            $('#gerarLote').click(function(){
		                if ($("[name^='proid[]']:checked").length<1) {
		                    alert('Selecione pelo menos um termo.');
		                    return false;
		                } else {
							$('#funcao').val('geraXls');
							$('#formulario').submit();
		                }
		                //$('#aba').val('macroGeraExcel');
		                //$('#formulario').submit();
		                //$('#conteudoPagina').html('<center><p><input type="button" value="Voltar" id="voltar" class="voltar" /></p></center>');
		            });
		
		            /*$('#selectAll').click(function () {
		                $("[name^='tcpid[]']").each(function(i,v){
		                    if($(v).attr('checked')){
		                        $(v).attr('checked', false);
		                    }else{
		                        $(v).attr('checked', true);
		                    }
		                });
		            });*/
		            $('#selectAll').click(function () {
		                $("[name^='proid[]']").each(function(i,v){
		                    if($(v).attr('checked')){
		                        $(v).attr('checked', false);
		                    }else{
		                        $(v).attr('checked', true);
		                    }
		                });
		            });
		
		            $('#voltar').live('click', function(){
		                document.location = 'ted.php?modulo=relatorio/exportacaoMacro&acao=A';
		            });
				
		            $(".verCO").click(function(){
		                var tcpid = $(this).attr("data-target-id")
		                    , $element;
		                if (tcpid) {
			                
		                    $element = $("#tr_tcpid_"+tcpid);
		                    if ($element.css("display") == 'none') {
		                    	$element.attr("style", "display:");
		                    	$('#spanDetalhe_'+tcpid).attr('class', 'glyphicon glyphicon-minus');
		                    } else {
		                    	$element.attr("style", "display:none;");
		                    	$('#spanDetalhe_'+tcpid).attr('class', 'glyphicon glyphicon-plus');
		                    }
		                }
		            });
		        });
		
		        $(window).resize(function(){
		            largura = (93*$(window).width())/100;
		            $('#divConteudo').css('width', largura);
		        });
		    </script>

				<div class="row col-md-12">
					<ol class="breadcrumb">
						<li><a href="ted.php?modulo=inicio&acao=C"><?=$_SESSION['sisdsc']; ?></a></li>
						<li class="active">Lista de Termos de Cooperação</li>
					</ol>
				    									        		    
				    	<div id="conteudoPagina">
				            <form id="formulario" class="form-listagem" name="formulario" method="post" action="ted.php?modulo=relatorio/exportacaoMacro&acao=A">
				                <input type="hidden" name="aba" id="aba" value="" />
				                <input type="hidden" name="funcao" id="funcao" value="" />
				                <div style="overflow:auto;" id="divConteudo">
				                    <table align="center" cellspacing="0" cellpadding="3" class="table table-striped table-bordered table-hover tabela-listagem">
				                        <thead>
				                        <tr>
				                            <?php if(empty($_REQUEST['lotid'])): ?>
				                                <th><input type="checkbox" name="selectAll" id="selectAll" class="selectAll"/></th>
				                            <?php else: ?>
				                                <th>Ação</th>
				                            <?php endif; ?>
				                            <th>UG PROPONENTE</th>
				                            <th>GESTÃO PROPONENTE</th>
				                            <th>RESPONSÁVEL 1 - PROPONENTE</th>
				                            <th>UG CONCEDENTE</th>
				                            <th>GESTAO CONCEDENTE</th>
				                            <th>RESPONSÁVEL 2 - CONCEDENTE</th>
				                            <th>NÚMERO ORIGINAL</th>
				                            <th>TIPO DE CADASTRO</th>
				                            <th>NÚMERO DO PROCESSO</th>
				                            <th>UG REPASSADORA</th>
				                            <th>GESTÃO REPASSADORA</th>
				                            <th>UG RECEBEDORA DO RECURSO</th>
				                            <th>GESTÃO RECEBEDORA</th>
				                            <th>TÍTULO</th>
				                            <th>INÍCIO DA VIGÊNCIA</th>
				                            <th>FIM DA VIGÊNCIA</th>
				                            <th>VALOR FIRMADO</th>
				                            <th>VALOR TOTAL</th>
				                            <th>DESCRIÇÃO</th>
				                            <th>JUSTIFICATIVA</th>
				                            <th>EXERCÍCIO 1</th>
				                            <th>EXERCÍCIO 2</th>
				                            <th>EXERCÍCIO 3</th>
				                            <th>EXERCÍCIO 4</th>
				                            <th>EXERCÍCIO 5</th>
				                            <th>EXERCÍCIO 6</th>
				                            <th>PRAZO 1</th>
				                            <th>PRAZO 2</th>
				                            <th>PRAZO 3</th>
				                            <th>PRAZO 4</th>
				                            <th>PRAZO 5</th>
				                            <th>% REAL</th>
				                            <?php if($qtdMaxPrevisao>0): ?>
				                                <?php for($x=1;$x<=$qtdMaxPrevisao;$x++): ?>
				                                    <th>EVENTO <?php echo $x; ?></th>
				                                    <th>ESFERA <?php echo $x; ?></th>
				                                    <th>PTRES <?php echo $x; ?></th>
				                                    <th>FONTE <?php echo $x; ?></th>
				                                    <th>PI <?php echo $x; ?></th>
				                                    <th>ND <?php echo $x; ?></th>
				                                    <th>VALOR ND <?php echo $x; ?></th>
				                                <?php endfor; ?>
				                            <?php endif; ?>
				                            <th>N° de Registro</th>
				                        </tr>
				                        </thead>
				                        <tbody>
				                        <?php foreach($rs as $k => $dado): ?>
				                            <?php if (fmod($k,2) == 0 && $_GET['req'] != 'gerarExcel') $marcado = '' ; else $marcado='#F7F7F7'; ?>
				                            <tr bgcolor="<?php echo $marcado; ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?php echo $marcado; ?>';">
				                                <?php if(empty($_REQUEST['lotid'])): ?>
				                                    <td>
				                                        <!-- img style="cursor:pointer;" src="../imagens/seta_filho.gif" class="verCO" data-target-id="<?php echo $dado['tcpid'] ?>" / -->
			                                        	<center>
				                                        	<span id="spanDetalhe_<?php echo $dado['tcpid'] ?>" class="glyphicon glyphicon-plus verCO" title="Detalhar" data-target-id="<?php echo $dado['tcpid'] ?>">
				                                        	</span>
			                                        	</center>
				                                    	<input type="hidden" name="tcpid[]" value="<?php echo $dado['tcpid']; ?>"/>
				                                    </td>
				                                <?php else: ?>
				                                    <td>
				                                        <!-- <img style="cursor:pointer;" src="../imagens/excluir.gif" class="excluirTermoLote" id="<?= $dado['tcpid'] ?>-<?= $_GET['lotid']?>" /> -->
				                                        <!-- img style="cursor:pointer;" src="../imagens/seta_filho.gif" class="verCO" data-target-id="<?php echo $dado['tcpid'] ?>" / -->
				                                        <!-- a href="javascript:void()" title="Detalhar" class="verCO" data-target-id="<?php echo $dado['tcpid'] ?>" -->
				                                        	<center>
					                                        	<span id="spanDetalhe_<?php echo $dado['tcpid'] ?>" class="glyphicon glyphicon-plus verCO" title="Detalhar" data-target-id="<?php echo $dado['tcpid'] ?>">
					                                        	</span>
				                                        	</center>
				                                        <!-- /a -->
				                                    </td>
				                                <?php endif; ?>
				                                <td><?php echo $dado['ungcodproponente']; ?></td>
				                                <td><?php echo $dado['gescodproponente']; ?></td>
				                                <td><?php echo $dado['cpfreplegalproponente']; ?></td>
				                                <td><?php echo $dado['ungcodconcedente']; ?></td>
				                                <td><?php echo $dado['gescodconcedente']; ?></td>
				                                <td><?php echo $dado['cpfreplegalconcedente']; ?></td>
				                                <td><?php echo $dado['tcpid']; ?></td>
				                                <td>6</td>
				                                <td><?php echo $dado['tcpid']; ?></td>
				                                <td>152734</td>
				                                <td>00001</td>
				                                <td><?php echo $dado['ungcodproponente']; ?></td>
				                                <td><?php echo $dado['gescodproponente']; ?></td>
				                                <td alt="<?php echo $dado['tcptitulo']; ?>" title="<?php echo $dado['tcptitulo']; ?>"><?php echo substr($dado['tcptitulo'], 0, 15); ?>...</td>
				                                <td><?php echo $dado['data_vigencia'] ? str_replace($arMesNum, $arMesDsc, formata_data($dado['data_vigencia'])) : ''; ?></td>
				                                <td><?php echo $dado['data_vigencia'] ? str_replace($arMesNum, $arMesDsc, makeDateSoma(formata_data($dado['data_vigencia']), 0, $dado['crdmesexecucao'])) : ''; ?></td>
				                                <td><?php echo $dado['valor_total'] ? str_replace('.',',',$dado['valor_total']) : ''; ?></td>
				                                <td><?php echo $dado['valor_total'] ? number_format($dado['valor_total'], 0, ".", "") : ''; ?></td>
				                                <td alt="<?php echo $dado['tcpobjetivoobjeto']; ?>" title="<?php echo $dado['tcpobjetivoobjeto']; ?>"><?php echo substr($dado['tcpobjetivoobjeto'], 0, 15); ?>...</td>
				                                <td alt="<?php echo $dado['tcpjustificativa']; ?>" title="<?php echo $dado['tcpjustificativa']; ?>"><?php echo substr($dado['tcpjustificativa'], 0, 15); ?>...</td>
				                                <td></td>
				                                <td></td>
				                                <td></td>
				                                <td></td>
				                                <td></td>
				                                <td></td>
				                                <td>360</td>
				                                <td></td>
				                                <td></td>
				                                <td></td>
				                                <td></td>
				                                <td>10000</td>
				
				                                <?php
				
				                                if($_REQUEST['lotid']){
				                                    $where[] = "pro.proid in (select loi.proid from ted.lotemacroitens loi where loi.proid = pro.proid and loi.loistatus = 'A')";
				                                }
				
				                                $sql = "SELECT *, substr(ndpcod, 1, 6) as natureza FROM ted.previsaoorcamentaria pro
														LEFT JOIN monitora.ptres ptr ON ptr.ptrid = pro.ptrid
														LEFT JOIN monitora.pi_planointerno pi on pi.pliid = pro.pliid
														LEFT JOIN public.naturezadespesa ndp ON ndp.ndpid = pro.ndpid
														WHERE pro.tcpid = {$dado['tcpid']}
														".(is_array($where) ? ' AND '.implode(' AND ',$where) : '')."
														ORDER BY pro.proid";
				                                $rsPrevisoes = $db->carregar($sql);
				                                ?>
				
				                                <?php if($qtdMaxPrevisao>0): ?>
				                                    <?php for($x=0;$x<$qtdMaxPrevisao;$x++): ?>
				
				                                        <?php if($rsPrevisoes[$x]['proid']>0): ?>
				                                            <td></td>
				                                            <td></td>
				                                            <td><?php echo $rsPrevisoes[$x]['ptres']; ?></td>
				                                            <td></td>
				                                            <td><?php echo $rsPrevisoes[$x]['plicod']; ?></td>
				                                            <td><?php echo $rsPrevisoes[$x]['natureza']; ?></td>
				                                            <td><?php echo str_replace('.','',$rsPrevisoes[$x]['provalor']); ?></td>
				                                        <?php else: ?>
				                                            <td>&nbsp;</td>
				                                            <td>&nbsp;</td>
				                                            <td>&nbsp;</td>
				                                            <td>&nbsp;</td>
				                                            <td>&nbsp;</td>
				                                            <td>&nbsp;</td>
				                                            <td>&nbsp;</td>
				                                        <?php endif; ?>
				
				                                    <?php endfor; ?>
				                                <?php endif; ?>
				                                <td><?php echo $k+1; ?></td>
				                            </tr>
				
				                            <?php if ('gerarExcel' != $_GET['req']): ?>
				                                <tr class="tr_sub" id="tr_tcpid_<?= $dado['tcpid']; ?>" style="display:none">
				                                	<td></td>
				                                    <td colspan="46">
				                                        <?php celulaOrcamentariaTable($dado['tcpid'], $_GET['lotid']); ?>
				                                    </td>
				                                </tr>
				                            <?php endif; ?>
				
				                        <?php endforeach; ?>
				                        </tbody>
				                    </table>
				                </div>
				            </form>
				            <br />
			                <center>
			                	<?php if(empty($_REQUEST['lotid'])): ?>
			                		<button type="button" class="btn btn-success" name="gerarLote" id="gerarLote">Gerar Lote</button>
		                		<?php endif; ?>
			                	<button type="button" class="btn btn-primary" name="voltar" id="voltar">Voltar</button>
			                </center>
				                
		    		    </div>
				    
				</div>
				
	        </center>
		<?php else: ?>
		    <center><b>Sem registros.</b>
		        <p><input type="button" value="Voltar" id="voltar" class="voltar" /></p></center>
		<?php endif;		
		
	}
	
	public function geraListaMacrosExcel(){
		global $db;
		
		header('Content-type: application/xls');
		header('Content-Disposition: attachment; filename="macro_termo_cooperacao_'.date('YmdHis').'.xls"');
		
		// Recupera termos para geração das macros
		$rs = recuperarLoteTermosTeste();
		//ver($rs);
		// Recupera quantidade maxima de previsão orçamentária do termo
		$qtdMaxPrevisao = contaQtdMaxPrevisaoOrcamentariaTeste();
		
		$arMesNum = array('/01/','/02/','/03/','/04/','/05/','/06/','/07/','/08/','/09/','/10/','/11/','/12/');
		$arMesDsc = array('/jan/','/fev/','/mar/','/abr/','/mai/','/jun/','/jul/','/ago/','/set/','/out/','/nov/','/dez/');
		$arRemoveCarcTexto = array('"',"'", "http://", "/", "(", ")");
		?>
		
		<?php if($rs): ?>
		
		    <table>
		        <thead>
		        <tr>
		            <th>UG PROPONENTE</th>
		            <th>GESTÃO PROPONENTE</th>
		            <th>RESPONSÁVEL 1 - PROPONENTE</th>
		            <th>UG CONCEDENTE</th>
		            <th>GESTAO CONCEDENTE</th>
		            <th>RESPONSÁVEL 2 - CONCEDENTE</th>
		            <th>NÚMERO ORIGINAL</th>
		            <th>TIPO DE CADASTRO</th>
		            <th>NÚMERO DO PROCESSO</th>
		            <th>UG REPASSADORA</th>
		            <th>GESTÃO REPASSADORA</th>
		            <th>UG RECEBEDORA DO RECURSO</th>
		            <th>GESTÃO RECEBEDORA</th>
		            <th>TÍTULO</th>
		            <th>INÍCIO DA VIGÊNCIA</th>
		            <th>FIM DA VIGÊNCIA</th>
		            <th>VALOR FIRMADO</th>
		            <th>VALOR TOTAL</th>
		            <th>DESCRIÇÃO</th>
		            <th>JUSTIFICATIVA</th>
		            <th>EXERCÍCIO 1</th>
		            <th>EXERCÍCIO 2</th>
		            <th>EXERCÍCIO 3</th>
		            <th>EXERCÍCIO 4</th>
		            <th>EXERCÍCIO 5</th>
		            <th>EXERCÍCIO 6</th>
		            <th>PRAZO 1</th>
		            <th>PRAZO 2</th>
		            <th>PRAZO 3</th>
		            <th>PRAZO 4</th>
		            <th>PRAZO 5</th>
		            <th>% REAL</th>
		            <?php if($qtdMaxPrevisao>0): ?>
		                <?php for($x=1;$x<=$qtdMaxPrevisao;$x++): ?>
		                    <th>EVENTO <?php echo $x; ?></th>
		                    <th>ESFERA <?php echo $x; ?></th>
		                    <th>PTRES <?php echo $x; ?></th>
		                    <th>FONTE <?php echo $x; ?></th>
		                    <th>PI <?php echo $x; ?></th>
		                    <th>ND <?php echo $x; ?></th>
		                    <th>VALOR ND <?php echo $x; ?></th>
		                <?php endfor; ?>
		            <?php endif; ?>
		            <th>N° de Registro</th>
		        </tr>
		        </thead>
		        <tbody>
		        <?php foreach($rs as $k => $dado): ?>
		            <tr>
		                <td><?php echo $dado['ungcodproponente']; ?></td>
		                <td><?php echo $dado['gescodproponente'] ? "'".$dado['gescodproponente']."" : ''; ?></td>
		                <td><?php echo $dado['cpfreplegalproponente'] ? "'".$dado['cpfreplegalproponente']."" : ''; ?></td>
		                <td><?php echo $dado['ungcodconcedente']; ?></td>
		                <td><?php echo $dado['gescodconcedente'] ? "'".$dado['gescodconcedente']."" : ''; ?></td>
		                <td><?php echo $dado['cpfreplegalconcedente'] ? "'".$dado['cpfreplegalconcedente']."" : ''; ?></td>
		                <td><?php echo $dado['tcpid']; ?></td>
		                <td>6</td>
		                <td><?php echo $dado['tcpid']; ?></td>
		                <td>152734</td>
		                <td>'00001</td>
		                <td><?php echo $dado['ungcodproponente']; ?></td>
		                <td><?php echo "'".$dado['gescodproponente']; ?></td>
		                <td><?php echo substr(str_replace($arRemoveCarcTexto, '', $dado['tcptitulo']), 0, 69); ?></td>
		                <td><?php echo $dado['data_vigencia'] ? str_replace($arMesNum, $arMesDsc, formata_data($dado['data_vigencia'])) : ''; ?></td>
		                <td><?php echo $dado['data_vigencia'] ? str_replace($arMesNum, $arMesDsc, makeDateSoma(formata_data($dado['data_vigencia']), 0, $dado['crdmesexecucao'])) : ''; ?></td>
		
		                <?php
		                // -- Ao chegar aqui, a execução sempre será do XLS
		                $valorTotal = $dado['valor_total'] ? str_replace('.',',',$dado['valor_total']) : '';
		                $valorTotalSIGEF = $dado['valor_total'] ? str_replace('.','',$dado['valor_total']) : '';
		                ?>
		
		                <td><?php echo $valorTotal; ?></td>
		                <td><?php echo $valorTotalSIGEF; ?></td>
		                <td><?php echo substr(str_replace($arRemoveCarcTexto, '', $dado['tcpobjetivoobjeto']), 0, 489); ?></td>
		                <td><?php echo substr(str_replace($arRemoveCarcTexto, '', $dado['tcpjustificativa']), 0, 349); ?></td>
		                <td></td>
		                <td></td>
		                <td></td>
		                <td></td>
		                <td></td>
		                <td></td>
		                <td>360</td>
		                <td></td>
		                <td></td>
		                <td></td>
		                <td></td>
		                <td>10000</td>
		
		                <?php
		
		                /**
		                 * Se vier [lotid] busca um lote existente
		                 * se não, busca o ultimo lote que acaba de ser criado
		                 */
		                if (isset($_GET['lotid'])) {
		                    $_lotid = (int) $_GET['lotid'];
		                } else {
		                    $_lotid = "(select lotid from ted.lotemacro order by lotid desc limit 1)";
		                }
		
		                $sql = "SELECT
		                            *, substr(ndpcod, 1, 6) as natureza FROM ted.previsaoorcamentaria pro
								LEFT JOIN monitora.ptres ptr ON ptr.ptrid = pro.ptrid
								LEFT JOIN monitora.pi_planointerno pi on pi.pliid = pro.pliid
								LEFT JOIN public.naturezadespesa ndp ON ndp.ndpid = pro.ndpid
								WHERE
								    pro.tcpid = {$dado['tcpid']} AND
									pro.proid IN (select lmi.proid from ted.lotemacroitens lmi where lmi.lotid = {$_lotid})
								ORDER BY pro.proid ASC";
		                //ver($sql, d);
		                $rsPrevisoes = $db->carregar($sql);

		                ?>
		
		                <?php if($qtdMaxPrevisao>0): ?>
		                    <?php for($x=0;$x<$qtdMaxPrevisao;$x++): ?>
		
		                        <?php if($rsPrevisoes[$x]['proid']>0): ?>
		                            <td></td>
		                            <td></td>
		                            <td><?php echo $rsPrevisoes[$x]['ptres']; ?></td>
		                            <td></td>
		                            <td><?php echo $rsPrevisoes[$x]['plicod']; ?></td>
		                            <td><?php echo $rsPrevisoes[$x]['natureza']; ?></td>
		                            <td><?php echo str_replace('.','',$rsPrevisoes[$x]['provalor']); ?></td>
		                        <?php else: ?>
		                            <td>&nbsp;</td>
		                            <td>&nbsp;</td>
		                            <td>&nbsp;</td>
		                            <td>&nbsp;</td>
		                            <td>&nbsp;</td>
		                            <td>&nbsp;</td>
		                            <td>&nbsp;</td>
		                        <?php endif; ?>
		
		                    <?php endfor; ?>
		                <?php endif; ?>
		                <td><?php echo $k+1; ?></td>
		            </tr>
		        <?php endforeach; ?>
		        </tbody>
		    </table>
		
		<?php endif;
	}
	
}