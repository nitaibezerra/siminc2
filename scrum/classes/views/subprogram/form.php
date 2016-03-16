<?php 

$entity = $this->entity;

$programas = $this->program;

global $db;
$sistemas = $db->carregar("SELECT sis.sisid AS codigo, sis.sisdsc AS descricao
                          FROM seguranca.sistema sis
                          WHERE sis.sisstatus = 'A'");

$sistemasDemanda = $db->carregar("select  
						s.sidid  AS codigo, 
						upper(s.sidabrev) || ' - ' || s.siddescricao AS descricao 
					from 
						demandas.sistemadetalhe s
					left join demandas.sistemacelula c on s.sidid = c.sidid  
					where  s.sidstatus = 'A'
					AND celid = 2 -- Celula do Daniel
					order by s.sidabrev");

  $useragent = $_SERVER['HTTP_USER_AGENT'];
 
  if (preg_match('|MSIE ([0-9].[0-9]{1,2})|',$useragent,$matched)) {
    $browser_version=$matched[1];
    $browser = 'IE';
  } elseif (preg_match( '|Opera/([0-9].[0-9]{1,2})|',$useragent,$matched)) {
    $browser_version=$matched[1];
    $browser = 'Opera';
  } elseif(preg_match('|Firefox/([0-9\.]+)|',$useragent,$matched)) {
    $browser_version=$matched[1];
    $browser = 'Firefox';
  } elseif(preg_match('|Chrome/([0-9\.]+)|',$useragent,$matched)) {
    $browser_version=$matched[1];
    $browser = 'Chrome';
  } elseif(preg_match('|Safari/([0-9\.]+)|',$useragent,$matched)) {
    $browser_version=$matched[1];
    $browser = 'Safari';
  } else {
    // browser not recognized!
    $browser_version = 0;
    $browser= 'other';
  }
//  print "browser: $browser $browser_version";


?>
<br />
<div class="well">
    <form id="formSaveSubProgram" name="formSaveSubProgram" method="POST" class="form-horizontal">
        <fieldset>
            <input type="hidden" name="subprgid" id="subprgid" value="<?php echo $entity['subprgid']['value'] ?>" />
            <div class="form-group has-warning">
                <label for="subprgdsc" class="col-lg-2 control-label">Nome</label>
                <div class="col-lg-10">
                    <input name="subprgdsc" id="subprgdsc" class="form-control" type="text" value="<?php echo $entity['subprgdsc']['value'] ?>" required="required"/>
                    <!--<span class="help-block">Nome do Projeto.</span>-->
                </div>
            </div>
            <div class="form-group has-warning">
                <label for="subprgcolor" class="col-lg-2 control-label">Cor</label>
                <div class="col-lg-10">
                    <input name="subprgcolor" id="subprgcolor" class="simple_color form-control" type="color" value="<?php echo $entity['subprgcolor']['value'] ?>" required="required"/>
                    <!--<span class="help-block">Nome do Projeto.</span>-->
                </div>
            </div>
            <div class="form-group has-warning" id="container_select_subprograma">
                <label for="prgid" class="col-lg-2 control-label">Projeto</label>
                <div class="col-lg-10">
                    <select name="prgid" id="prgid" class="form-control chosen-select" required="required" data-placeholder="Selecione">
                        <option value=""></option>
                        <?php foreach ($programas as $programa): ?>
                            <option <?php if($entity['prgid']['value'] == $programa['prgid']) echo 'selected="selected"' ?> value="<?php echo $programa['prgid'] ?>"><?php echo $programa['prgdsc'] ?></option>
                        <?php endforeach; ?>
                    </select>
                </div>
            </div>
            <div class="form-group has-warning" id="container_select_subprograma">
                <label for="sisid" class="col-lg-2 control-label">Sistema</label>
                <div class="col-lg-10">
                    <select name="sisid" id="sisid" class="form-control chosen-select" required="required" data-placeholder="Selecione">
                        <option value=""></option>
                        <?php foreach ($sistemas as $sistema): ?>
                        <option <?php if($entity['sisid']['value'] == $sistema['codigo']) echo 'selected="selected"' ?> value="<?php echo $sistema['codigo'] ?>"><?php echo $sistema['descricao'] ?></option>
                        <?php endforeach; ?>
                    </select>
                </div>
            </div>
            <div class="form-group has-warning" id="container_select_subprograma">
                <label for="sidid" class="col-lg-2 control-label">Sistema do demandas</label>
                <div class="col-lg-10">
                    <select name="sidid" id="sidid" class="form-control chosen-select" required="required" data-placeholder="Selecione">
                        <option value=""></option>
                        <?php foreach ($sistemasDemanda as $sistemaDemanda): ?>
                            <option <?php if($entity['sidid']['value'] == $sistemaDemanda['codigo']) echo 'selected="selected"' ?> value="<?php echo $sistemaDemanda['codigo'] ?>"><?php echo $sistemaDemanda['descricao'] ?></option>
                        <?php endforeach; ?>
                    </select>
                    <span class="help-block">Ao criar uma demanda este é o sistema que ficará visível no demandas.</span>
                </div>
            </div>
            <div class="form-group">
                <div class="col-lg-10 col-lg-offset-2">
                    <?php if($entity['subprgid']['value']) : ?>
                    <button class="btn btn-danger" id="buttonCancel" >Cancelar</button>
                    <button class="btn btn-info" id="buttonSave" >Atualizar</button>
                    <?php else: ?>
                    <button class="btn btn-warning" id="buttonCancel" type="reset">Limpar</button> 
                    <button class="btn btn-success" id="buttonSave">Inserir</button>
                    <button class="btn btn-primary" id="buttonSearch">Buscar</button>
                    <?php endif ?>
                </div>
            </div>
        </fieldset>
    </form>
</div>
<script lang="javascript">
    $('#prghrsprint').mask('999');
    
    $('#formSaveSubProgram #buttonCancel').click(function() {
        $.renderAjax({controller: 'subprogram', action: 'form', container: 'container_form_subprogram'});
        return false;
    });
    
    $('#formSaveSubProgram #buttonSave').click(function() {
        $(this).parents('form:first').saveAjax({controller: 'subprogram', action: 'save', functionSucsess: 'saveSucsess'});
        return false;
    });
    
    $('#formSaveSubProgram #buttonSearch').click(function() {
        $(this).parents('form:first').find('.has-error').removeClass('has-error');
        search($(this).parents('form:first'));
        return false;
    });
    
    function saveSucsess(element)
    {
        $.renderAjax({controller: 'subprogram', action: 'form', container: 'container_form_subprogram'});
        $(element).searchAjax({controller: 'subprogram', action: 'list', container: 'container_list_subprogram'});
    }
    
    function search(element)
    {
        $(element).searchAjax({controller: 'subprogram', action: 'list', container: 'container_list_subprogram'});
    }
    
    setTimeout(function() {
        for (var selector in config) {
            $(selector).chosen(config[selector]);
        }
        
        
    }, 200);
    
    <?php if($browser != 'Chrome' && $browser != 'Opera'): ?>
        jQuery(document).ready(function() {
            jQuery('.simple_color').simpleColor({
                cellWidth:20,
                cellHeight:20,
                border:'1px solid #333333'
            });
        });
    <?php endif ?>
</script>