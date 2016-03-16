<?php 

//Declaração de Objetos
$unidadeGestora = new Ted_Model_UnidadeGestora();
$termoExecDesc = new Ted_Model_TermoExecucaoDescentralizada();
$representanteLegal = new Ted_Model_RepresentanteLegal();
$justificativa = new Ted_Model_Justificativa();
$previsaoOrcamentaria = new Ted_Model_PrevisaoOrcamentaria();

//Capturando proponente
$ungCodProp = $termoExecDesc->capturaProponente();
//Capturando dados da ung proponente
$dadosUngProp = $unidadeGestora->pegaUnidade($ungCodProp);

//capturando dados do representante legal da ung proponente
$representanteProponente = $representanteLegal->recuperaResponsavelUG($ungCodProp);
$responsavel = $representanteLegal->areaTecnicaResponsavel($_GET['ted']);

//Capturando concedente
$ungCodConc = $termoExecDesc->capturaConcedente();
$coordenacaoResponsavel = $representanteLegal->coordenacaoResponsavel($_GET['ted']);

//capturando dados da ung concedente
$dadosUngConc = $unidadeGestora->pegaUnidade($ungCodConc['ungcodconcedente']);
//capturando dados do representante legal da ung concedente
$representanteConcedente = $representanteLegal->recuperaResponsavelUG($ungCodConc['ungcodconcedente']);

//Capturando dados da justificativa
$dadosJustificativa = $justificativa->capturaDadosJustificativa();

//Capturando lista previsão orçamentaria do termo
$listaPO = $previsaoOrcamentaria->buscaPrevisaoOrcamentariaPDF();

//Capturando prazo para o objeto termo
$prazoObjeto = $previsaoOrcamentaria->capturaPrazoTotalPO();

//Capturando o reitor do atual termo
$reitor = $termoExecDesc->capturaReitor();

//Captura contagem do termo (não entendi)
$rsCountSolAlt = $termoExecDesc->capturaContagemTermo();
?>

<section class="col-md-12">
	<form class="form-horizontal" enctype="multipart/form-data" name="<?= $this->element->getName(); ?>" id="<?= $this->element->getId(); ?>" action="<?= $this->element->getAction(); ?>" method="<?= $this->element->getMethod(); ?>" role="form">
		<?= $this->element->tcpid; ?>
		<section class="well">
			<table class="col-md-12 table-condensed table-hover table-responsive">
				<tr>
					<th class="col-md-2 text-right" style="background-color: #BBB;">Nº do Termo:</td>
					<td class="col-md-6">&nbsp; <?= ($rsCountSolAlt > 0 ? $_GET['ted'].'.'. $rsCountSolAlt : $_GET['ted'])?></td>
					<?php if ($ungCodConc['ungcodconcedente'] == UG_FNDE && in_array ($ungCodConc['ungcodpoliticafnde'], array (UG_SECADI,UG_SETEC,UG_SEB))) { ?>
					<th class="col-md-2 text-right" style="background-color: #BBB;">Nº do Processo:</th>
					<td class="col-md-4">&nbsp; <?= $ungCodConc['tcpnumprocessofnde']?></td>
					<?php } else { ?>
					<th class="col-md-2 text-right"></th>
					<td class="col-md-2"></td>
					<?php } ?>
				</tr>
			</table>
		</section>
		<br>			
	    <table class="col-md-12 table-condensed table-bordered table-hover table-responsive">
	    	<thead>
	    		<tr class="well" >
	    			<th  colspan="4" class="text-center">DADOS DO ÓRGÃO OU ENTIDADE PROPONENTE</th>
	    		</tr>
	    	</thead>
	    	<tbody>
	    		<tr>
	    			<th>Cód. Und. Gestora</th>
	    			<th>Cód. da Gestão</th>
	    			<th>CNPJ</th>
	    			<th>Razão Social</th>
	    		</tr>
	    		<tr>
	    			<td><?= $dadosUngProp['ungcod'];?></td>
	    			<td><?= $dadosUngProp['gescod']; ?></td>
	    			<td><?= formatar_cnpj($dadosUngProp['ungcnpj']); ?></td>
	    			<td><?= $dadosUngProp['descricao']; ?></td>
	    		</tr>
	    		<tr>
	    			<th colspan="2">Endereço</th>
	    			<th>Bairro ou Distrito</th>	
	    			<th>Município</th>
	    		</tr>
	    		<tr>
	    			<td colspan="2"><?= $dadosUngProp['ungendereco']; ?></td>
	    			<td><?= $dadosUngProp['ungbairro']; ?></td>
	    			<td><?= $dadosUngProp['municipio']; ?></td>
	    		</tr>
	    		<tr>
	    			<th>UF</th>
	    			<th>CEP</th>
	    			<th>Telefone</th>
	    			<th>E-Mail</th>
	    		</tr>
	    		<tr>
	    			<td><?= $dadosUngProp['estuf']; ?></td>
	    			<td><?= $dadosUngProp['ungcep']; ?></td>
	    			<td><?= $dadosUngProp['ungfone']; ?></td>
	    			<td><?= $dadosUngProp['ungemail']; ?></td>
	    		</tr>	    		
	    	</tbody>	    		    
	    </table>		
	    <br>
		<table class="col-md-12 table-condensed table-bordered table-hover table-responsive">
	    	<thead>
	    		<tr class="well" >
	    			<th  colspan="4" class="text-center">REPRESENTANTE LEGAL DO ORGÃO OU ENTIDADE PROPONENTE</th>
	    		</tr>
	    	</thead>
	    	<tbody>
	    		<tr>
	    			<th colspan="2">CPF</th>
	    			<th colspan="2">Nome do Representante Legal</th>	    				    			
	    		</tr>
	    		<tr>
	    			<td colspan="2"><?= formatar_cpf($representanteProponente['usucpf']); ?></td>
	    			<td colspan="2"><?= $representanteProponente['usunome']; ?></td>	    				    			
	    		</tr>
	    		<tr>
	    			<th colspan="2">Endereço</th>
	    			<th>Bairro ou Distrito</th>	
	    			<th>Município</th>
	    		</tr>
	    		<tr>
	    			<td colspan="2"><?= $representanteProponente['endereco']; ?></td>
	    			<td><?= $representanteProponente['bairro']; ?></td>
	    			<td><?= $representanteProponente['municipio']; ?></td>
	    		</tr>
	    		<tr>
	    			<th>UF</th>
	    			<th>CEP</th>
	    			<th>Telefone</th>
	    			<th>E-Mail</th>
	    		</tr>
	    		<tr>
	    			<td><?= $representanteProponente['estado']; ?></td>
	    			<td><?= $representanteProponente['endcep']; ?></td>
	    			<td><?= $representanteProponente['fone']; ?></td>
	    			<td><?= $representanteProponente['usuemail']; ?></td>
	    		</tr>	  
	    		<tr>
	    			<th colspan="2">Nº da Cédula da CI</th>
	    			<th>Órgão Expeditor</th>	
	    			<th>Cargo</th>
	    		</tr>
	    		<tr>
	    			<td colspan="2"><?= $representanteProponente['numeroidentidade']; ?></td>
	    			<td><?= $representanteProponente['entorgaoexpedidor']; ?></td>
	    			<td><?= $representanteProponente['usufuncao']; ?></td>	    			
	    		</tr>
                <?php if ($responsavel): ?>
                <tr>
                    <th colspan="3">Área Técnica Responsável</th>
                    <th>CPF</th>
                </tr>
                <tr>
                    <td colspan="3"><?= $responsavel['usunome'] ?></td>
                    <td><?= formatar_cpf($responsavel['usucpf']) ?></td>
                </tr>
                <?php endif; ?>
	    	</tbody>	    		    
	    </table>		
	    <br>
	    <table class="col-md-12 table-condensed table-bordered table-hover table-responsive">
	    	<thead>
	    		<tr class="well" >
	    			<th  colspan="4" class="text-center">DADOS DO ÓRGÃO OU ENTIDADE CONCEDENTE</th>
	    		</tr>
	    	</thead>
	    	<tbody>
	    		<tr>
	    			<th>Cód. Und. Gestora</th>
	    			<th>Cód. da Gestão</th>
	    			<th>CNPJ</th>
	    			<th>Razão Social</th>
	    		</tr>
	    		<tr>
	    			<td><?= $dadosUngConc['ungcod'];?></td>
	    			<td><?= $dadosUngConc['gescod']; ?></td>
	    			<td><?= formatar_cnpj($dadosUngConc['ungcnpj']); ?></td>
	    			<td><?= $dadosUngConc['descricao']; ?></td>
	    		</tr>
	    		<tr>
	    			<th colspan="2">Endereço</th>
	    			<th>Bairro ou Distrito</th>	
	    			<th>Município</th>
	    		</tr>
	    		<tr>
	    			<td colspan="2"><?= $dadosUngConc['ungendereco']; ?></td>
	    			<td><?= $dadosUngConc['ungbairro']; ?></td>
	    			<td><?= $dadosUngConc['municipio']; ?></td>
	    		</tr>
	    		<tr>
	    			<th>UF</th>
	    			<th>CEP</th>
	    			<th>Telefone</th>
	    			<th>E-Mail</th>
	    		</tr>
	    		<tr>
	    			<td><?= $dadosUngConc['estuf']; ?></td>
	    			<td><?= $dadosUngConc['ungcep']; ?></td>
	    			<td><?= $dadosUngConc['ungfone']; ?></td>
	    			<td><?= $dadosUngConc['ungemail']; ?></td>
	    		</tr>	    		
	    	</tbody>	    		    
	    </table>		
	    <br>
	    <table class="col-md-12 table-condensed table-bordered table-hover table-responsive">
	    	<thead>
	    		<tr class="well" >
	    			<th  colspan="4" class="text-center">REPRESENTANTE LEGAL DO ORGÃO OU ENTIDADE CONCEDENTE</th>
	    		</tr>
	    	</thead>
	    	<tbody>
	    		<tr>
	    			<th colspan="2">CPF</th>
	    			<th colspan="2">Nome do Representante Legal</th>	    				    			
	    		</tr>
	    		<tr>
	    			<td colspan="2"><?= formatar_cpf($representanteConcedente['usucpf']);?></td>
	    			<td colspan="2"><?= $representanteConcedente['usunome']; ?></td>	    				    			
	    		</tr>
	    		<tr>
	    			<th colspan="2">Endereço</th>
	    			<th>Bairro ou Distrito</th>	
	    			<th>Município</th>
	    		</tr>
	    		<tr>
	    			<td colspan="2"><?= $representanteConcedente['endereco']; ?></td>
	    			<td><?= $representanteConcedente['bairro']; ?></td>
	    			<td><?= $representanteConcedente['municipio']; ?></td>
	    		</tr>
	    		<tr>
	    			<th>UF</th>
	    			<th>CEP</th>
	    			<th>Telefone</th>
	    			<th>E-Mail</th>
	    		</tr>
	    		<tr>
	    			<td><?= $representanteConcedente['estado']; ?></td>
	    			<td><?= $representanteConcedente['endcep']; ?></td>
	    			<td><?= $representanteConcedente['fone']; ?></td>
	    			<td><?= $representanteConcedente['usuemail']; ?></td>
	    		</tr>	  
	    		<tr>
	    			<th colspan="2">Nº da Cédula da CI</th>
	    			<th>Órgão Expeditor</th>	
	    			<th>Cargo</th>
	    		</tr>
	    		<tr>
	    			<td colspan="2"><?= $representanteConcedente['numeroidentidade']; ?></td>
	    			<td><?= $representanteConcedente['entorgaoexpedidor']; ?></td>
	    			<td><?= $representanteConcedente['usufuncao']; ?></td>	    			
	    		</tr>
                <?php if ($coordenacaoResponsavel): ?>
                    <tr>
                        <th colspan="2">Coordenação Responsável</th>
                        <th>CPF</th>
                    </tr>
                    <tr>
                        <td colspan="2"><?= $coordenacaoResponsavel['usunome']; ?></td>
                        <td><?= formatar_cpf($coordenacaoResponsavel['usucpf']); ?></td>
                    </tr>
                <?php endif; ?>
	    	</tbody>
	    </table>		
	    <br>		
	    <table class="col-md-12 table-condensed table-bordered table-hover table-responsive">
	    	<thead>
	    		<tr class="well" >
	    			<th  colspan="2" class="text-center">OBJETO E JUSTIFICATIVA DA DESCENTRALIZAÇÃO DO CRÉDITO</th>
	    		</tr>
	    	</thead>
	    	<tbody>
	    		<tr>
	    			<th colspan="2">Identificação (Título/Objeto da Despesa)</th>	    				    				    			
	    		</tr>
	    		<tr>
	    			<td colspan="2"><?= $dadosJustificativa['identificacao'];?></td>
	    		</tr>
	    		<tr>
	    			<th colspan="2">Objetivo</th>	    			
	    		</tr>
	    		<tr>
	    			<td colspan="2"><?= $dadosJustificativa['objetivo']; ?></td>
	    		</tr>
	    		<tr>
	    			<th>UG/Gestão Repassadora</th>
	    			<th>UG/Gestão Recebedora</th>	    				    			
	    		</tr>
	    		<tr>
	    			<td><?= $dadosJustificativa['ugrepassadora']; ?></td>
	    			<td><?= $dadosJustificativa['ugrecebedora']; ?></td>
	    		</tr>	  
	    		<tr>
	    			<th colspan="2">Justificativa (Motivação/Clientela/Cronograma Físico)</th>	    				    			
	    		</tr>
	    		<tr>
	    			<td colspan="2"><?= $dadosJustificativa['justificativa']; ?></td>
	    		</tr>  	
	    		<tr>
	    			<td colspan="2">
	    			<?php 
	    				if($ungCodConc['ungcodconcedente'] == UG_FNDE 
							&& in_array($ungCodConc['ungcodpoliticafnde'], array(UG_SECADI,UG_SETEC,UG_SEB)))
						{
					?>
							<p>
								I - Integra este termo, independentemente de transcrição, o Plano de Trabalho e o Termo de Referência, cujos dados ali contidos acatam os partícipes e se comprometem em cumprir, sujeitando-se às normas da Lei Complementar nº 101/2000, Lei nº 8.666, de 21 de junho de 1993, no que couber, Lei nº 4.320/1964, Lei nº 10.520/2002, Decreto nº 93.872/1986 e o de nº 6.170, de 25 de julho de 2007, Portaria Interministerial no 507, de 24 de novembro de 2011, Portaria Conjunta MP/MF/CGU nº 8, de 7 de novembro de 2012, bem como o disposto na Resolução CD/FNDE nº 28/2013.<br/>
							</p>
							<br>		    	
							<p>
		    					II - constituem obrigações da CONCEDENTE:<br/>
								a) efetuar a transferência dos recursos financeiros previstos para a execução deste Termo, na forma estabelecida no Cronograma de Desembolso constante do Plano de Trabalho; <br/>
		    				</p>
		    				<br>
		    				<p>
								III - constituem obrigações do GESTOR DO PROGRAMA:<br/>
								a) orientar, supervisionar e cooperar com a implantação das ações objeto deste Termo;<br/>
								b) acompanhar as atividades de execução, avaliando os seus resultados e reflexos;<br/>
								c) analisar o relatório de cumprimento do objeto do presente Termo;<br/>
	    					</p>
	    					<br>
					    	<p> 
								IV - constituem obrigações da PROPONENTE:<br/>
								a) solicitar ao gestor do projeto senha e login do SIMEC;<br/>
								b) solicitar à UG concedente senha e login do SIGEFWEB, no caso de recursos enviados pelo FNDE;<br/>
								c) promover a execução do objeto do Termo na forma e prazos estabelecidos no Plano de Trabalho;<br/>
								d) aplicar os recursos discriminados exclusivamente na consecução do objeto deste Termo;<br/>
								e) permitir e facilitar ao Órgão Concedente o acesso a toda documentação, dependências e locais do projeto;<br/>
								f) observar e exigir, na apresentação dos serviços, se couber, o cumprimento das normas específicas que regem a forma de execução da ação a que os créditos estiverem vinculados;<br/>
								g) manter o órgão Concedente informado sobre quaisquer eventos que dificultem ou interrompam o curso normal de execução do Termo;<br/>
								h) devolver os saldos dos créditos orçamentários descentralizados e não empenhados, bem como os recursos financeiros não utilizados, conforme norma de encerramento do correspondente exercício financeiro;<br/>
								i) emitir o relatório descritivo de cumprimento do objeto proposto;<br/>
								j) comprovar o bom e regular emprego dos recursos recebidos, bem como dos resultados alcançados;<br/>
								k) assumir todas as obrigações legais decorrentes de contratações necessárias à execução do objeto do termo;<br/>
								l) solicitar ao gestor do projeto , quando for o caso, a prorrogação do prazo para cumprimento do objeto em até quinze (15) dias antes do término previsto no termo de execução descentralizada, ficando tal prorrogação condicionada à aprovação por aquele;<br/>
								m) a prestação de contas dos créditos descentralizados devem integrar as contas anuais do órgão Proponente a serem apresentadas aos órgãos de controle interno e externo, conforme normas vigentes;<br/>
								n) apresentar relatório de cumprimento do objeto pactuado até 60 dias após o término do prazo para cumprimento do objeto estabelecido no Termo.<br/>
					    	</p>
						<?php 
						}else
						{
						?>							
	    					<p>
	    						I - Integra este termo, independentemente de transcrição, o Plano de Trabalho e o Termo de Referência, cujos dados ali contidos acatam os partícipes e se comprometem em cumprir, sujeitando-se às normas da Lei Complementar nº 101/2000, Lei nº 8.666, de 21 de junho de 1993, no que couber, Lei nº 4.320/1964, Lei nº 10.520/2002, Decreto nº 93.872/1986 e o de nº 6.170, de 25 de julho de 2007, Portaria Interministerial no 507, de 24 de novembro de 2011, Portaria Conjunta MP/MF/CGU nº 8, de 7 de novembro de 2012, bem como o disposto na Resolução CD/FNDE nº 28/2013.<br/>
	    					</p>
	    					<br>
					    	<p> 
								II - constituem obrigações da CONCEDENTE:<br/>
								a) efetuar a transferência dos recursos financeiros previstos para a execução deste Termo, na forma estabelecida no Cronograma de Desembolso constante do Plano de Trabalho;<br/>
								b) orientar, supervisionar e cooperar com a implantação das ações objeto deste Termo;<br/>
								c) acompanhar as atividades de execução, avaliando os seus resultados e reflexos;<br/>
								d) analisar o relatório de cumprimento do objeto do presente Termo;<br/>
					    	</p>
					    	<br>
					    	<p> 
								III - constituem obrigações da PROPONENTE:<br/>
								a) solicitar ao gestor do projeto senha e login do SIMEC;<br/>
								b) solicitar à UG concedente senha e login do SIGEFWEB, no caso de recursos enviados pelo FNDE;<br/>
								c) promover a execução do objeto do Termo na forma e prazos estabelecidos no Plano de Trabalho;<br/>
								d) aplicar os recursos discriminados exclusivamente na consecução do objeto deste Termo;<br/>
								e) permitir e facilitar ao Órgão Concedente o acesso a toda documentação, dependências e locais do projeto;<br/>
								f) observar e exigir, na apresentação dos serviços, se couber, o cumprimento das normas específicas que regem a forma de execução da ação a que os créditos estiverem vinculados;<br/>
								g) manter o órgão Concedente informado sobre quaisquer eventos que dificultem ou interrompam o curso normal de execução do Termo;<br/>
								h) devolver os saldos dos créditos orçamentários descentralizados e não empenhados, bem como os recursos financeiros não utilizados, conforme norma de encerramento do correspondente exercício financeiro;<br/>
								i) emitir o relatório descritivo de cumprimento do objeto proposto;<br/>
								j) comprovar o bom e regular emprego dos recursos recebidos, bem como dos resultados alcançados;<br/>
								k) assumir todas as obrigações legais decorrentes de contratações necessárias à execução do objeto do termo;<br/>
								l) solicitar ao gestor do projeto , quando for o caso, a prorrogação do prazo para cumprimento do objeto em até quinze (15) dias antes do término previsto no termo de execução descentralizada, ficando tal prorrogação condicionada à aprovação por aquele;<br/>
								m) a prestação de contas dos créditos descentralizados devem integrar as contas anuais do órgão Proponente a serem apresentadas aos órgãos de controle interno e externo, conforme normas vigentes;<br/>
								n) apresentar relatório de cumprimento do objeto pactuado até 60 dias após o término do prazo para cumprimento do objeto estabelecido no Termo.<br/>
					    	</p>
	    			<?php 
						}
	    			?>
	    			</td>
	    		</tr>	
	    	</tbody>	    		    
	    </table>	
	    <br>
	    
	    <table class="col-md-12 table-condensed table-bordered table-hover table-responsive">
	    	<thead>
	    		<tr class="well" >
	    			<th  colspan="8" class="text-center">PREVISÃO ORÇAMENTÁRIA</th>
	    		</tr>
	    		<tr style="font-size:12px;">
	    			<th>Ano</th>
	    			<th>Programa de Trabalho</th>
	    			<th>Ação</th>
	    			<th>Plano Interno</th>
	    			<th>Descrição da Ação constante da LOA</th>
	    			<th>Natureza da Despesa</th>
	    			<th>Mês da Liberação</th>
	    			<th>Valor (em R$ 1,00)</th>
	    		</tr>
	    	</thead>
	    	<tbody style="font-size:11px;">
	    	<?php 
	    	if($listaPO)
			{
				$arNotaCredito = array ();
				$totalPrevisao = count($listaPO) - 1;
                $total = 0;
	    		foreach ($listaPO as $k => $po)
				{
                    $total += $po['valor'];

					if (!in_array($po['notacredito'], $arNotaCredito ))
					{
						if ($subTotal > 0)
						{
							echo '
									<tr bgcolor="#f0f0f0">
										<td colspan="8" align="right">
											<table width="550">
												<tr>
													<td align="left"><b>Nota de Crédito(' . ($ncAnterior ? $ncAnterior : 'Não informado') . ')</b>&nbsp;</td>
													<td align="right"><b>Subtotal</b>&nbsp;</td>
													<td align="right" width="110"><b>R$ ' . formata_valor ( $subTotal ) . '</b></td>
												</tr>
											</table>
										</td>
									</tr>
								';
						}
						array_push ($arNotaCredito, $po['notacredito'] );
						$subTotal = 0;
						$ncAnterior = $po['notacredito'];
					}
					echo 
					'					
					<tr>
						<td>'.$po['proanoreferencia'] . '</td>
						<td>'.$po['plano_trabalho'] . '</td>
						<td>'.$po['acao'] . '</td>
						<td>'.$po['plano_interno'] . '</td>
						<td>'.$po['acao_loa'] . '</td>
						<td>'.$po['nat_despesa'] . '</td>
						<td>'.Ted_Utils_Model::mes_extenso($po['crdmesliberacao']).'</td>
						<td style="text-align:right;">R$ '.$po['provalor'].'</td>
					</tr>
					';
					$subTotal = $subTotal + $po['valor'];
					
					if ($totalPrevisao == $k)
					{
						echo'
							<tr bgcolor="#f0f0f0">
								<td colspan="8" align="right">
									<table width="550">
										<tr>
											<td align="left"><b>Nota de Crédito(' . ($ncAnterior ? $ncAnterior : 'ano não informado') . ')</b>&nbsp;</td>
											<td align="right"><b>Subtotal</b>&nbsp;</td>
											<td align="right" width="110"><b>R$ ' . formata_valor ( $subTotal ) . '</b></td>
										</tr>
									</table>
								</td>
							</tr>
						';
					}
				}    	
				
				echo '
				<tr>
		  			<th style="font-size:12px;" colspan="3">Prazo para o cumprimento do objeto</th>
		  			<td style="font-size:12px;" colspan="1"><b>'.($prazoObjeto['crdmesexecucao'] ? $prazoObjeto['crdmesexecucao'] . '&nbsp;meses' : '').'</b>&nbsp;</td>
		    		<th style="font-size:12px;" colspan="2" class="text-right">TOTAL</th>
		    		<td style="font-size:12px;" colspan="2" align="right"><b>'.number_format2($total).'</b></td>
				</tr>
				';
			}			    		    	
	    	?>	    		  		
	    	</tbody>	    		    
	    </table>	
	    <br>
	    <section class="well">
	    <!-- ASSINATURA DO TERMO -->
		    <?php 
		    
		    //Guarda o estado atual do termo
		    $estadoTermoAtual = $termoExecDesc->pegarEstadoAtualTermo();
		    
		    //mostra assinatura somente quando termo é aprovado pelo representante legal do proponente
		    $mostraAssinaturaReitor = (!in_array($estadoTermoAtual, array(EM_CADASTRAMENTO, TERMO_AGUARDANDO_APROVACAO_GESTOR_PROP, EM_APROVACAO_DA_REITORIA)));

		    if ($reitor && $mostraAssinaturaReitor) {
		    	$stAnaliseReitor = "Autorizado pelo(a) {$reitor['usunome']} no dia {$reitor['htddata']} às {$reitor['hora']} <br/>";
		    }

		    //Captura presidente e secretário do termo
		    $presSec = $termoExecDesc->capturaSecretarioTermo();
		    $rsPresidente = $presSec[0];
		    $rsSecretario = $presSec[1];
		    
		    
		    $stAnaliseSecretaria = '';
		    
		    $arrayPull = array(
		    		EM_CADASTRAMENTO,
		    		TERMO_AGUARDANDO_APROVACAO_GESTOR_PROP,
		    		EM_APROVACAO_DA_REITORIA,
		    		EM_ANALISE_OU_PENDENTE,
		    		AGUARDANDO_APROVACAO_DIRETORIA,
		    		AGUARDANDO_APROVACAO_SECRETARIO,
		    		TERMO_EM_ANALISE_ORCAMENTARIA_FNDE,
		    		AGUARDANDO_APROVACAO_DIRETORIA,
		    );
		    
		    /**
		     * mostra assinatura somente quando termo é
		     * autorizado pelo representante legal do concedente
		     * ou
		     * encaminhado para validação da diretoria do FNDE
		    */
		    $mostraAssConcedente = (!in_array($estadoTermoAtual, $arrayPull));
		    
		    if ($rsSecretario && $mostraAssConcedente) {
		    	if ($rsPresidente) {
		    		$stAnaliseSecretaria = "Autorizado pelo(a) presidente(a) {$rsSecretario['usunome']} no dia {$rsSecretario['htddata']} às {$rsSecretario['hora']}";
		    	} else {
		    		$stAnaliseSecretaria = "Autorizado pelo(a) secretário(a) {$rsSecretario['usunome']} no dia {$rsSecretario['htddata']} às {$rsSecretario['hora']}";
		    	}
		    }
		    
		    echo '
				<table id="assinatura" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="100%">
				  <tr>
				    <td>&nbsp;</td>
				  </tr>
				  <tr>
				    <td align="center">
				    	' . $stAnaliseReitor . '
				    	' . $stAnaliseSecretaria . '
				    </td>
				  </tr>
				  <tr>
				    <td>&nbsp;</td>
				  </tr>
				  <tr>
		    		<td colspan="2" style="text-align:center; font-weight: bold;">'.Ted_Utils_Model::recuperaDataGeraPdf().'</td>
				  </tr>
				  <tr>
				    <td>&nbsp;</td>
				  </tr>
				</table>
	    	';
		    ?>	    
	    </section>	    	   
		<hr>
		<div class="well form-group">    	
			<div class=" col-md-10">			
				<button type="submit" class="btn btn-success" name="requisicao" value="gerarPDF" id="submit"><span class="glyphicon glyphicon-list-alt"></span> Gerar PDF</button>					    			
			</div>
		</div>		
	</form>
</section>