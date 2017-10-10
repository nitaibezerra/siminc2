
<form class="form-horizontal" name="filtroTed" id="filtroTed" action="<?= $this->element->getAction(); ?>" method="<?= $this->element->getMethod(); ?>" role="form">
    <?= $this->element->ungcod; ?>
    <?= $this->element->export; ?>
    <div class="form-group">
        <label class="control-label col-md-2" for="unicod">Unidade Orçamentária (UO) Concedente:</label>
        <div class="col-md-10">
            <?= $this->element->unicod; ?>
        </div>
    </div>

    <div class="form-group ">
        
        <label class="control-label col-md-2" for="tcpid">Número do TED <?php echo SIGLA_SISTEMA; ?>:</label>
        
        <div class="col-md-10">
            <?= $this->element->tcpid; ?>
        </div>
    </div>

    <div class="form-group ">

        <label class="control-label col-md-2" for="tcpnumtransfsiafi">Número de Transferência SIAFI:</label>

        <div class="col-md-10">
            <?= $this->element->tcpnumtransfsiafi; ?>
        </div>
    </div>

    <div class="form-group">        
        <label class="control-label col-md-2" for="message">Unidade Gestora (UG) Proponente:</label>
        
        <div class="col-md-10">
            <?= $this->element->ungcodproponente; ?>
        </div>
    </div>
    <div class="form-group">        
        <label class="control-label col-md-2" for="ungcodconcedente">Unidade Gestora (UG) Concedente:</label>        
        <div class="col-md-10">
            <?= $this->element->ungcodconcedente; ?>
        </div>
    </div>
    <div class="form-group">        
		<label class="control-label col-md-2" for="esdid">Situação do Termo:</label>        
        <div class="col-md-10">
            <?= $this->element->esdid; ?>
        </div>
    </div>

    <?php

    if (isset($_POST['vencimento']) && !empty($_POST['vencimento'])) {
        switch ($_POST['vencimento']) {
            case 30: $active30 = 'active'; $checked30= 'checked'; break;
            case 60: $active60 = 'active'; $checked60= 'checked'; break;
            case -1: $active0 = 'active'; $checked0= 'checked'; break;
            case -60: $activeMenos60 = 'active'; $checkedMenos60= 'checked'; break;
            default:
                $active30 = '';
                $active60 = '';
                $active0 = '';
                $checked30 = '';
                $checked60 = '';
                $checked0 = '';
                $activeTodos = 'active'; 
                $checkedTodos= 'checked';
        }
    }

    ?>

    <div class="form-group">
        <label class="control-label col-md-2">Vencimento em:</label>
        <div class="col-md-10">
            <div class="btn-group" data-toggle="buttons">
                <label class="btn btn-default <?=$activeTodos?>">
                    <input type="radio" name="vencimento" id="vencitodos" value="" <?=$checkedTodos?>>Todos
                </label>
                <label class="btn btn-default <?=$active30?>">
                    <input type="radio" name="vencimento" id="venci30" value="30" <?=$checked30?>>30 dias
                </label>
                <label class="btn btn-default <?=$active60?>">
                    <input type="radio" name="vencimento" id="venci60" value="60" <?=$checked60?>> 60 dias
                </label>
                <label class="btn btn-default <?=$active0?>">
                    <input type="radio" name="vencimento" id="venci1" value="-1" <?=$checked0?>> Vencidos
                </label>
                <label class="btn btn-default <?=$activeMenos60?>">
                    <input type="radio" name="vencimento" id="venci1" value="-60" <?=$checkedMenos60?>> Vencidos + 60 dias
                </label>
            </div>
        </div>
    </div>

    <hr />
    <div class="form-group">
    	<div class="col-md-offset-2">
    		<button type="submit" class="btn btn-primary" name="search" id="search">Pesquisar</button>
    		<button type="reset" class="btn btn-warning" id="clear">Limpar</button>	
    		<button type="submit" class="btn btn-info" id="exportarXls">Exportar XLS</button>
    	</div>
    </div>
    
</form>