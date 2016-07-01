<section class="well">
	<h4 class="text-center">Unidade Gestora Concedente</h4>
</section>
<form class="well form-horizontal"
      name="<?=$this->element->getName(); ?>"
      id="<?=$this->element->getId(); ?>"
      action="<?= $this->element->getAction(); ?>"
      method="<?= $this->element->getMethod(); ?>"
      role="form" novalidate>
	
	<?= $this->element->tcpid; ?>
    <div class="form-group">
        <label class="control-label col-md-2" for="ungcod">Selecionar Concedente:</label>
        <div class="col-md-10">
            <?= $this->element->ungcodconcedente; ?>
        </div>
    </div>
    <section id="blocked">

        <?php if ($this->element->ungcodpoliticafnde): ?>
            <div id="fndeblocked" class="form-group">
                <label class="control-label col-md-2" for="fnderespcod">Responsável pela Política:</label>
                <div class="col-md-10">
                    <?= $this->element->ungcodpoliticafnde; ?>
                </div>
            </div>
        <?php endif; ?>

	    <div class="form-group ">        
	        <label class="control-label col-md-2" for="unicod">Código da Unidade Orçamentária:</label>
	        
	        <div class="col-md-10">
	            <?= $this->element->unicod; ?>
	        </div>
	    </div>	    
	    <div class="form-group">        
	        <label class="control-label col-md-2" for="gescod">Código de Gestão:</label>
	        
	        <div class="col-md-10">
	            <?= $this->element->gescod; ?>
	        </div>
	    </div>
	    <div class="form-group">        
	        <label class="control-label col-md-2" for="ungcnpj">CNPJ:</label>
	        <div class="col-md-10">
	            <?= $this->element->ungcnpj; ?>
	        </div>
	    </div>
	    <div class="form-group">        
			<label class="control-label col-md-2" for="ungendereco">Endereço:</label>
	        <div class="col-md-10">
	            <?= $this->element->ungendereco; ?>
	        </div>
	    </div>
	    <div class="form-group">        
			<label class="control-label col-md-2" for="ungbairro">Bairro:</label>
	        <div class="col-md-10">
	            <?= $this->element->ungbairro; ?>
	        </div>
	    </div>
	    <div class="form-group">        
			<label class="control-label col-md-2" for="estuf">UF:</label>
	        <div class="col-md-10">
	            <?= $this->element->estuf; ?>
	        </div>
	    </div>
	    <div class="form-group">        
			<label class="control-label col-md-2" for="muncod">Município:</label>
	        <div class="col-md-10 muncod">
	            <?= $this->element->muncod; ?>
	        </div>
	    </div>
	    <div class="form-group">        
			<label class="control-label col-md-2" for="ungcep">CEP:</label>
	        <div class="col-md-10">
	            <?= $this->element->ungcep; ?>
	        </div>
	    </div>
	    <div class="form-group">
            <label class="control-label col-md-2" for="ungddd">DDD:</label>
            <div class="col-md-3">
                <?= $this->element->ungddd; ?>
            </div>

            <label class="control-label col-md-2" for="ungfone">Telefone:</label>
            <div class="col-md-5">
                <?= $this->element->ungfone; ?>
            </div>
	    </div>
	    <div class="form-group">
			<label class="control-label col-md-2" for="ungemail">E-mail:</label>
	        <div class="col-md-10">
	            <?= $this->element->ungemail; ?>
	        </div>
	    </div>

        <!-- espaço para área técnica responsavel -->
        <div class="form-group">
            <div class="col-md-12">
                <label class="col-md-12 text-center" for="nomecoordenacao">Área Técnica Responsável</label>
            </div>
        </div>
        <div class="form-group">
            <label class="control-label col-md-2" for="nomecoordenacao">Nome da Coordenação:</label>
            <div class="col-md-10">
                <?= $this->element->corid; ?>
                <?= $this->element->nomecoordenacao; ?>
            </div>
        </div>
        <div class="form-group">
            <label class="control-label col-md-2" for="dddcoordenacao">DDD da Coordenação:</label>
            <div class="col-md-1">
                <?= $this->element->dddcoordenacao; ?>
            </div>
            <label class="control-label col-md-4" for="telefonecoordenacao">Telefone da Coordenação:</label>
            <div class="col-md-5">
                <?= $this->element->telefonecoordenacao; ?>
            </div>
        </div>
        <!-- FIM espaço para área técnica responsavel -->

	    <div class="form-group">
	    	<div class="col-md-12">
	    		<table class="table table-condensed">
			    	<thead>
			    		<tr><th colspan="5" class="text-center">Representante Legal</th></tr>
			    		<tr>
			    			<th>CPF</th>
			    			<th>Nome</th>
			    			<th>Perfil</th>			    			
			    			<th>E-mail</th>
			    		</tr>
			    	</thead>
			    	<tbody>
			    		<tr>
			    			<td id=""><input class="form-control" type="text" name="usucpf" id="usucpf" value="" onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);" onblur="this.value=mascaraglobal('###.###.###-##',this.value);" /></td>
			    			<td id=""><input class="form-control" type="text" name="usunome" id="usunome" value="" /></td>
			    			<td id=""><input class="form-control" type="text" name="rplegal" disabled id="rplegal" value="Representante Legal" /></td>
			    			<td id=""><input class="form-control" type="text" name="usuemail" id="usuemail" value="" /></td>
			    		</tr>
			    	</tbody>
			    </table>	
	    	</div>
	    </div>

        <!-- espaço para representante legal substituto do proponente -->
        <div class="form-group">
            <div class="col-md-12">
                <table class="table table-condensed">
                    <thead>
                    <tr>
                        <th colspan="4" class="text-center">Representante Legal Substituto</th>
                    </tr>
                    <tr>
                        <th><label for="cpf">CPF</labe></th>
                        <th><label for="nome">Nome</labe></th>
                        <th><label for="funcao">Função</labe></th>
                        <th><label for="email">E-mail</labe></th>
                    </tr>
                    </thead>
                    <tbody>
                    <tr>
                        <td id="div_cpf">
                            <?= $this->element->rlid; ?>
                            <?= $this->element->cpf; ?>
                        </td>
                        <td id="div_nome">
                            <?= $this->element->nome; ?>
                        </td>
                        <td id="div_funcao">
                            <?= $this->element->funcao; ?>
                        </td>
                        <td id="div-email">
                            <?= $this->element->email; ?>
                        </td>
                    </tr>
                    </tbody>
                </table>
            </div>
        </div>
    </section>
    <hr />
    <div class="form-group">
    	<div class="col-md-offset-2">
    		<button type="button" class="btn btn-warning" name="cancel" id="cancel">Cancelar</button>
    		<button type="submit" class="btn btn-primary" name="submit" id="submit">Gravar</button>
    		<button type="submit" class="btn btn-success" name="submitcontinue" id="submitcontinue">Gravar e Continuar</button>    			
    	</div>
    </div>
</form>