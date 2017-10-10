<?php 
$termoExecDesc = new Ted_Model_TermoExecucaoDescentralizada();
$ungCodConc = $termoExecDesc->capturaConcedente();
?>
<script type="text/javascript">
$(function(){
    $("[for='tcptipoemenda-S'], [for='tcptipoemenda-N']").attr("class", "checkbox-inline");
})
</script>
<form class="well form-horizontal"
      name="<?=$this->element->getName(); ?>"
      id="<?=$this->element->getId(); ?>"
      action="<?= $this->element->getAction(); ?>"
      method="<?= $this->element->getMethod(); ?>"
      role="form">
	
	<?= $this->element->tcpid; ?>
	<?= $this->element->justid; ?>

    <?php if ($this->element->tipoemenda) : ?>
        <div class="form-group">
            <label class="control-label col-md-2" for="tipoemenda">É do tipo Emenda?</label>
            <div class="col-md-10">
                <?= $this->element->tipoemenda; ?>
            </div>
        </div>

        <div class="form-group div-emenda">
            <label class="control-label col-md-2" for="emeid">Emenda:</label>
            <div class="col-md-10 select-emenda">
                <?= $this->element->emeid; ?>
            </div>
        </div>
    <?php endif; ?>

    <div class="form-group">
        <label class="control-label col-md-2" for="identificacao">Identificação (Título / Objeto da despesa):</label>
        <div class="col-md-10">        	
            <?= $this->element->identificacao; ?>
            <div id="counter-identificacao" class=""></div>
        </div>
    </div>

   	<div id="fndeblocked" class="form-group">
    	<label class="control-label col-md-2" for="objetivo">Objetivo:</label>
    	<div class="col-md-10">
    		<?= $this->element->objetivo;?>
            <div id="counter-objetivo" class=""></div>
    	</div>
    </div>

    <div class="form-group ">        
        <label class="control-label col-md-2" for="ugrepassadora">UG/Gestão Repassadora:</label>
        <div class="col-md-10">
            <?= $this->element->ugrepassadora; ?>
	        </div>
	</div>

	<div class="form-group">        
		<label class="control-label col-md-2" for="ugrecebedora">UG/Gestão Recebedora:</label>
	    <div class="col-md-10">
	    	<?= $this->element->ugrecebedora; ?>
	    </div>
	</div>
	<div class="form-group">        
		<label class="control-label col-md-2" for="justificativa">Justificativa (Motivação / Clientela / Cronograma físico):</label>
	    <div class="col-md-10">
	    	<?= $this->element->justificativa; ?>
            <div id="counter-justificativa" class=""></div>
	    </div>
	</div>
	<div class="form-group">        
		<label class="control-label col-md-2" for="endereco">Relações entre as Partes:</label>        
	    <div class="col-md-10">
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
								a) solicitar ao gestor do projeto senha e login do <?php echo SIGLA_SISTEMA; ?>;<br/>
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
								a) solicitar ao gestor do projeto senha e login do <?php echo SIGLA_SISTEMA; ?>;<br/>
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
		</div>
	</div>	    
    <hr />
    <div class="form-group">
    	<div class="col-md-offset-2">
    		<button type="button" class="btn btn-warning" name="cancel" id="cancel">Cancelar</button>
    		<button type="submit" class="btn btn-primary" name="submit" id="submit">Gravar</button>
    		<button type="submit" class="btn btn-success" name="submitcontinue" id="submitcontinue">Gravar e Continuar</button>    			
    	</div>
    </div>
    
</form>